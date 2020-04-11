"""Helper functions for commands.
"""
import json
import os
import platform
import subprocess
import shlex

import qmk.keymap


def create_make_command(keyboard, keymap, target=None):
    """Create a make compile command

    Args:

        keyboard
            The path of the keyboard, for example 'plank'

        keymap
            The name of the keymap, for example 'algernon'

        target
            Usually a bootloader.

    Returns:

        A command that can be run to make the specified keyboard and keymap
    """
    make_args = [keyboard, keymap]

    if target:
        make_args.append(target)

    return ['make', ':'.join(make_args)]

def docker_wrap_command(command, privileged=False):
    """TODO:
add docs
add this stuff...
	if [ "$(uname)" = "Linux" ] || docker-machine active >/dev/null 2>&1; then
		usb_args="--privileged -v /dev:/dev"

    	--user $(id -u):$(id -g)
    """
    command = ['docker', 'run', '--rm', '-it',
                '-w', '/qmk_firmware',
                '-v', '%s:/qmk_firmware' % os.getcwd(),
                'qmkfm/base_container'] + command
    return command

def compile_configurator_json(user_keymap, bootloader=None):
    """Convert a configurator export JSON file into a C file

    Args:

        configurator_filename
            The configurator JSON export file

        bootloader
            A bootloader to flash

    Returns:

        A command to run to compile and flash the C file.
    """
    # Write the keymap C file
    qmk.keymap.write(user_keymap['keyboard'], user_keymap['keymap'], user_keymap['layout'], user_keymap['layers'])

    # Return a command that can be run to make the keymap and flash if given
    if bootloader is None:
        return create_make_command(user_keymap['keyboard'], user_keymap['keymap'])
    return create_make_command(user_keymap['keyboard'], user_keymap['keymap'], bootloader)


def parse_configurator_json(configurator_file):
    """Open and parse a configurator json export
    """
    user_keymap = json.load(configurator_file)

    return user_keymap


def run(command, *args, **kwargs):
    """Run a command with subprocess.run
    """
    platform_id = platform.platform().lower()

    if isinstance(command, str):
        raise TypeError('`command` must be a non-text sequence such as list or tuple.')

    if 'windows' in platform_id:
        safecmd = map(shlex.quote, command)
        safecmd = ' '.join(safecmd)
        command = [os.environ['SHELL'], '-c', safecmd]

    return subprocess.run(command, *args, **kwargs)
