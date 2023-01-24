# Game

A quick game I'm making for the web with C++!

## Platform Support

Here's the platform support status:

__Key__:
| Icon | Meaning               |
| :--: | :-------------------- |
|  🟢  | Full Support          |
|  🟠  | Some (in-dev) support |
|  🟡  | Untested              |
|  🔴  | Untested + unplanned  |

| Platform          | Status |
| :---------------- | :----: |
| Linux             |   🟢   |
| WASM (Emscripten) |   🟠   |
| Windows           |   🟡   |
| MacOS             |   🟡   |

## Compiling

Here's a script to compile for linux:

```sh
#!/bin/bash

# Auto-generate configure and other files.
./autogen.sh

# Configure the project.
./configure

# Build the project and dependencies.
make
```

And here's how to compile for the web (emscripten):

```sh
#!/bin/bash

# Auto-generate configure and other files.
./autogen.sh

# Configure the project.
emconfigure ./configure

# Build the project and dependencies.
emmake make
```
