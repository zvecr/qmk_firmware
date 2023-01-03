"""List the keyboards currently defined within QMK
"""
from milc import cli

import qmk.keyboard


@cli.argument('--resolve_defaults', arg_only=True, action='store_false', help='Parse any "DEFAULT_FOLDER" within keyboards rules.mk')
@cli.subcommand("List the keyboards currently defined within QMK")
def list_keyboards(cli):
    """List the keyboards currently defined within QMK
    """
    for keyboard_name in qmk.keyboard.list_keyboards(cli.args.resolve_defaults):
        print(keyboard_name)
