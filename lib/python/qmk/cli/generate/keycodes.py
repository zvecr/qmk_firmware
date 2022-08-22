"""Used by the make system to generate keycodes.h from keycodes_{version}.json
"""
from pathlib import Path

from milc import cli

from qmk.constants import GPL2_HEADER_C_LIKE, GENERATED_HEADER_C_LIKE
from qmk.json_schema import deep_update, json_load
from qmk.commands import dump_lines
from qmk.path import normpath


def _validate(spec):
    # no duplicate keycodes
    keycodes = []
    for value in spec['keycodes'].values():
        keycodes.append(value['key'])
        keycodes.extend(value.get('aliases', []))
    duplicates = set([x for x in keycodes if keycodes.count(x) > 1])
    if duplicates:
        cli.log.error(f'Keycode spec contains duplicate keycodes! ({",".join(duplicates)})')
        exit(1)

    # all groups exist
    groups = set(spec['groups'])
    bad_groups = set()
    for value in spec['keycodes'].values():
        group = value.get('group', 'basic')
        if group not in groups:
            bad_groups.add(group)
    if bad_groups:
        cli.log.warning(f'Keycode spec contains group/s ({",".join(bad_groups)}) that are not declared!')


def _build(version):
    """Build keycode data from the requested spec file
    """
    data_dir = Path('data/constants/')

    file = data_dir / f'keycodes_{version}.json'
    if not file.exists():
        cli.log.error(f'Requested keycode spec ({version}) is invalid!')
        exit(1)

    # Load base
    spec = json_load(file, True)

    # Merge in fragments
    fragments = data_dir.glob(f'keycodes_{version}_*.json')
    for file in fragments:
        deep_update(spec, json_load(file, True))

    # Sort?
    spec['keycodes'] = dict(sorted(spec['keycodes'].items()))

    # Validate?
    _validate(spec)

    return spec


def _generate(lines, version):
    """Generate keycode header from the requested spec file
    """
    keycodes = _build(version)

    lines.append('')
    lines.append('// Ranges')
    for key, value in keycodes["ranges"].items():
        lo, mask = map(lambda x: int(x, 16), key.split("/"))
        hi = lo + mask
        define = value.get("defines")[0]
        lines.append(f'#define {define.ljust(30)} 0x{lo:04x}')
        lines.append(f'#define {(define + "_MAX").ljust(30)} 0x{hi:04x}')

    lines.append('')
    lines.append('// Keycodes')
    for key, value in keycodes["keycodes"].items():
        lines.append(f'#define {value.get("key")} {key}')

    lines.append('')
    lines.append('// Alias')
    for key, value in keycodes["keycodes"].items():
        temp = value.get("key")
        for alias in value.get("aliases", []):
            lines.append(f'#define {alias.ljust(10)} {temp}')


@cli.argument('-v', '--version', arg_only=True, required=True, help='Version of keycodes to generate.')
@cli.argument('-o', '--output', arg_only=True, type=normpath, help='File to write to')
@cli.argument('-q', '--quiet', arg_only=True, action='store_true', help="Quiet mode, only output error messages")
@cli.subcommand('Used by the make system to generate keycodes.h from keycodes_{version}.json', hidden=True)
def generate_keycodes(cli):
    """Generates the keycodes.h file.
    """

    # Build the keycodes.h file.
    keycodes_h_lines = [GPL2_HEADER_C_LIKE, GENERATED_HEADER_C_LIKE, '#pragma once']

    if cli.args.version != 'legacy':
        _generate(keycodes_h_lines, cli.args.version)
    else:
        # TODO: Ideally this would instead generate from 'keycodes_legacy.json'
        keycodes_h_lines.append('#include "data/templates/keycodes/legacy/keycode.h"')
        keycodes_h_lines.append('#include "data/templates/keycodes/legacy/quantum_keycodes.h"')

    # Show the results
    dump_lines(cli.args.output, keycodes_h_lines, cli.args.quiet)
