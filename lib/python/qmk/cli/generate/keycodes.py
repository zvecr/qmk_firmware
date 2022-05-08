"""Used by the make system to generate keycodes.h from keycodes_{version}.json
"""
from pathlib import Path

from milc import cli

from qmk.constants import GPL2_HEADER_C_LIKE, GENERATED_HEADER_C_LIKE
from qmk.json_schema import json_load
from qmk.commands import dump_lines
from qmk.path import normpath


def _generate(lines, version):
    """Build keycode from the requested spec file
    """
    spec = Path(f'data/constants/keycodes_{version}.json')
    if not spec.exists():
        cli.log.error(f'Requested keycode spec ({version}) is invalid:!')
        exit(1)

    keycodes = json_load(spec)
    for key, value in keycodes["keycodes"].items():
        lines.append(f'#define {value.get("key")} {key}')


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
