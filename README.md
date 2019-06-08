## About

Simple containerisation experiment using Linux features.

## Use

- Linux only -- namespaces aren't supported elsewhere
- Try `./build_and_run.sh run /bin/bash` to launch the container with an interactive shell

## Scripts

- you'll need to `chmod +x script_name.sh` to add permissions for each to be executed

- gcc_install: You need gcc to build_and_run. This script installs it.

- download_img_base: Pulls a base Ubuntu filesystem for constructing our container image.

- build_and_run: Compiles, and runs if compile success. Passes through your command line args.

- docker_run: Launches a Docker container with Ubuntu, and mounts this project inside. This allows
us to change & test system settings without worrying about doing damage.

- change_dir: a utility for making sure we execute in the correct container

## TODO

- CGroups
- User & Network namespaces
