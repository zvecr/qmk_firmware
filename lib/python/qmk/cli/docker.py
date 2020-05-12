"""TODO.
"""
import subprocess
import platform
import os

from milc import cli

@cli.argument('docker_params', nargs='*', arg_only=True, help='Args to pass to qmk global command.')
@cli.subcommand("TODO:")
def docker(cli):
    """TODO.
    """
    if cli.args.docker_params:
        cmd = ['bin/qmk', '--config-file', '/qmk.ini'] + cli.args.docker_params
    else:
        cmd = ['/bin/bash']
        cli.log.info("Press Control+D to exit.")

    usb_args = []
    platform_id = platform.platform().lower()
    if 'linux' in platform_id:
        usb_args=['--privileged', '-v', '/dev:/dev']

    subprocess.run(['docker', 'run', '--rm', '-it', *usb_args,
        '--user', '%s:%s' % (os.getuid(), os.getgid()),
        '-v', '%s:/qmk_firmware' % os.getcwd(),
        '-v', '%s:/qmk.ini' % cli.find_config_file(), 
        '-w', '/qmk_firmware',
        'qmkfm/base_container'] + cmd)
