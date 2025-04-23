Nix for OTB
----------------------------

`Nix <https://nixos.org/>`__ provides a unique approach to package management that ensures reproducible, reliable, and conflict-free software environments. For OTB users and developers, Nix offers several advantages:

- **Pure reproducibility**: Exact same OTB environment across different machines and time
- **Isolated dependencies**: No conflicts with system packages or other projects
- **Declarative configuration**: Environment defined in code that can be version controlled
- **Rollbacks**: Easy to switch between different versions of OTB or its dependencies
- **Multi-user support**: Different users can have different OTB configurations without interference


Installing Nix
--------------

Before installing OTB with Nix, install the Nix package manager if it isn't already installed:

::

   # Install Nix using the recommended installer
   curl --proto '=https' --tlsv1.2 -sSf -L https://install.determinate.systems/nix | sh -s -- install

Alternatively, visit `Zero to Nix <https://zero-to-nix.com/start/install/>`__ for detailed installation instructions for your specific operating system.

Installing OTB via Nix
----------------------

The OTB package in Nix is available on both stable and unstable channel.

To install OTB via Nix:

For temporary usage in a development environment:

::

  # Install the latest stable OTB version
   nix shell nixpkgs#otb

   # Or pass a specific channel such as stable version
   nix shell github:NixOS/nixpkgs/nixos-24.11#otb

   # Or unstable version (mostly the bleeding edge version)
   nix shell github:NixOS/nixpkgs/nixpkgs-unstable#otb

If using NixOS, you can add OTB to system configuration by adding the following to `configuration.nix`:

::

   environment.systemPackages = with pkgs; [
     otb
   ];


Development Environments
------------------------

**Using Python with OTB**:


To use OTB with Python, you'll need to set up the correct environment variables.
Here are development environment examples that configure the environment properly:

**Using shell.nix**:

::

   { pkgs ? import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/nixos-24.11.tar.gz") {
     }
   }:pkgs.mkShell {
     buildInputs = [
       pkgs.otb
     ];
     # we add this environment variables for otb to be importable via python
     postShellHook = ''
       export OTB_APPLICATION_PATH="${pkgs.otb}/lib/otb/applications"
       export PYTHONPATH="$PYTHONPATH:${pkgs.otb}/lib/otb/python"
     '';
   }

Save this to a file named `shell.nix` and run `nix-shell` in the same directory to enter
an environment with OTB properly configured for Python use.

**Using flake.nix (recommended)**:

::

   {
     description = "OTB development environment";

     inputs = {
       nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
     };

     outputs = { self, nixpkgs }:
     let
       system = "x86_64-linux";  # Change to match your system
       pkgs = nixpkgs.legacyPackages.${system};
     in {
       devShells.${system}.default = pkgs.mkShell {
         packages = with pkgs; [
           otb
         ];
        # we add this environment variables for otb to be importable via python
         postShellHook = ''
           export OTB_APPLICATION_PATH="${pkgs.otb}/lib/otb/applications"
           export PYTHONPATH="$PYTHONPATH:${pkgs.otb}/lib/otb/python"
         '';
       };
     };
   }

Save this to a file named `flake.nix` and run `nix develop` in the same directory to enter the development environment.

Once inside the Nix shell or flake development environment, you can import and use OTB in Python.

**Advantages of using Nix flakes**:

- **Reproducibility**: Flakes lock all dependencies, ensuring that your environment remains consistent across different machines and over time.
- **Explicit dependencies**: Flakes make dependencies explicit and always use the exact same Nixpkgs version.
- **Composability**: Flakes can be easily composed together, making it simpler to create complex development environments.
- **Better caching**: Outputs are properly cached, leading to faster builds.
- **Standardized interface**: Flakes provide a standardized way to interact with Nix packages and environments.
- **Simplified sharing**: Environments can be easily shared with others, ensuring they get exactly the same setup.

After installation, you can use OTB applications directly from the command line or import the OTB modules in Python.

Notes:
~~~~~~
  - When setting up a new project with this flake, you should initialize a Git repository in your
    project directory using ``git init`` and add the nix files to be tracked using ``git add *.nix``.
    This is necessary because Nix flakes require a Git repository to track changes properly.

Complex Development Environments
--------------------------------

One advantage of Nix is the ability to install OTB with different Nix packages seamlessly.
One can find all available packages in the Nix package repository at `Nixpkgs Search <https://search.nixos.org/packages>`__.

**Using flake.nix with Python virtual environment**:

::

    {
      description = "OTB development environment";

      inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
      };

      outputs =
        { self, nixpkgs }:
        let
          system = "x86_64-linux";  # Change to match your system
          pkgs = nixpkgs.legacyPackages.${system};
          python = pkgs.python3;
          pyPkgs = python.pkgs;

          nativePkgs = with pkgs; [
            otb
            # gqis
            # git
            # Add other packages as needed
          ];
          extraPyPkgs = with pyPkgs; [
            # rasterio
            # geopandas
            # scikit-learn
            # scikit-image
            # Add other python packages as needed
          ];
          pyEnv = python.withPackages (ps: with ps; extraPyPkgs);

        in
        {
          devShells.${system}.default = pkgs.mkShell {
           # this creates a .venv directory inside the project folder to be consumed by IDE
            venvDir = "./.venv";
            packages = with pkgs; [
              pyPkgs.venvShellHook
              nativePkgs
              pyEnv
            ];
            postShellHook = ''
              export OTB_APPLICATION_PATH="${pkgs.otb}/lib/otb/applications"
              export PYTHONPATH="$PYTHONPATH:${pkgs.otb}/lib/otb/python"
            '';
          };
        };
    }

This setup creates a development environment that combines OTB with Python packages managed by Nix,
while also providing a virtual environment structure that can be useful for IDE integration and
organization of additional development tools.

Using direnv and nix-direnv with flakes
--------------------------------------

To make your development environment even more convenient, you can combine flakes with `direnv <https://direnv.net/docs/hook.html>`__ and
`nix-direnv <https://github.com/nix-community/nix-direnv>`__. This automates the process of loading your development environment whenever you
enter your project directory, saving you from manually running ``nix develop`` each time.

Setting up direnv with nix-direnv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. **Create a .envrc file** in your project directory with the following content:

::

    if ! has nix_direnv_version || ! nix_direnv_version 3.0.6; then
      source_url "https://raw.githubusercontent.com/nix-community/nix-direnv/3.0.6/direnvrc" "sha256-RYcUJaRMf8oF5LznDrlCXbkOQrywm0HDv1VjYGaJGdM="
    fi
    use flake

2. **Allow direnv** to run in this directory:

::

      direnv allow

3. **Add this to your shell configuration file** (``.bashrc``, ``.zshrc``, etc.) to hook direnv into your shell:

::

      eval "$(direnv hook bash)"  # or zsh, fish, etc.

Now, whenever you enter your project directory, direnv will automatically load your Nix flake
environment defined in ``flake.nix``. When you exit the directory, it will unload the environment,
keeping your global environment clean.

Benefits of direnv with nix-direnv
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- **Automatic environment activation**: No need to manually run ``nix develop`` every time you start working
- **Improved caching**: nix-direnv provides better caching of Nix environments than plain direnv
- **Project-specific environments**: Different projects can have different dependencies without conflicts
- **Seamless workflow**: Your terminal will always have the right tools and environment variables for the current project

This approach works perfectly with the flake.nix file shown earlier, creating a fully reproducible
and automatically loaded development environment for OTB with all the necessary paths and dependencies
configured.

To use OTB with Python in this environment, you don't need to do anything special - just enter the
directory and start coding. The environment variables set in the flake's ``postShellHook`` will
ensure Python can find the OTB modules.

Notes:
~~~~~~

   -  If you're using VSCode or other IDEs, you may need additional configuration to make them
      recognize the direnv-loaded environment. Consider installing the appropriate extensions
      for your editor to support direnv integration.


Advanced Configuration of OTB with Nix
---------------------------------------
For more advanced usage such as building from source with remote modules, docker build,
you may refer to `otb-nix <https://github.com/daspk04/otb-nix/tree/main>`__.
