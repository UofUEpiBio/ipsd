# IPSD

> IPSD is unfinished software; see `TODO.md`.

IPSD (or interpreter passthrough server daemon) is software that enables clients
to run script code remotely over an HTTP connection, without putting the server
(and its stewards) in grave peril. The server daemon allows access to a
*pre-provisioned* environment over a network. This is ideal for workshops,
hackathons, or technical product showcases.

IPSD is written in C, and provides various security features.

- Syscall Restriction
- I/O Timeouts
- Resource Utilization Restrictions

## Installation

### Prelude

The server this is running on should be a Unix. Preferably, your platform should
be compatible with one of the syscall restriction mechanisms that IPSD supports.
See the below table.

##### Syscall Restriction Mechanisms

**Method**        |**Strength**|**Platform**
:-----------------|:-----------|:-----------
`sandbox_init(3)` |Strong      |macOS
`seccomp(2)`      |Strong      |Linux
`pledge(2)`       |Strong      |OpenBSD, FreeBSD

### Compilation

IPSD is built with [Meson](https://mesonbuild.com), and thus can be built
by following the formula you may be familiar with from building previous
pieces of software written in Meson.

Before building, however, you must configure IPSD with the languages you
want it to support, and point it towards the appropriate environments for
each of them. The reason this isn't done at runtime is because IPSD is
meant to be run inside of an automatically provisioned, and therefore
static, system.

You can find a full list of the configuration options as they're visible to
the build in `meson.options`, or you can see the `CONFIGURING.md` file for a
more in-depth look.

Finally, you can compile.

```bash
# Get
git clone https://github.com/UofUEpiBio/ipsd.git
cd ipsd

# Compile
meson setup build ${YOUR_CONFIGURATION_STRING}
cd build
ninja

# Install, optional
sudo ninja install
```

## Usage

Using IPSD is easy; you just start the daemon:

```
ipsd
```

If you want to daemonize it, you can use your operating system's provided
facility to do that.

It is recommended to place IPSD behind a reverse proxy, and run all
communication over HTTPS/TLS. You can use [Nginx](https://nginx.org/) or
[Caddy](https://caddyserver.com/) for this.

## Disclaimer

The authors of this software, nor any organization they are associated with,
may be held responsible--legally and morally--for any damages caused by this
software.
