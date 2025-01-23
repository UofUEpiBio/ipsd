#include "python.h"
#include <Python.h>
#include <aux/complain.h>
#include <aux/platform.h>
#include <stdbool.h>

#define PYTHON_PRELUDE                                                                             \
    "import matplotlib\n"                                                                          \
    "matplotlib.use('Agg')\n"                                                                      \
    "import matplotlib.pyplot as plt\n"                                                            \
    "from io import BytesIO\n"                                                                     \
    "import base64\n"                                                                              \
    ""                                                                                             \
    "plt.rcParams['figure.dpi'] = 100\n"                                                           \
    ""                                                                                             \
    "def __ipsd_internal_show_inline():\n"                                                         \
    "   buf = BytesIO()\n"                                                                         \
    "   plt.savefig(buf, format='png')\n"                                                          \
    "   buf.seek(0)\n"                                                                             \
    "   img_base64 = base64.b64encode(buf.read()).decode('utf-8')\n"                               \
    "   buf.close()\n"                                                                             \
    ""                                                                                             \
    "   encoded = f'data:image/png;base64,{img_base64}'\n"                                         \
    "   print('__ipsd_rich_content_image:' + encoded)\n"                                           \
    ""                                                                                             \
    "   plt.clf()\n"                                                                               \
    ""                                                                                             \
    "plt.show = __ipsd_internal_show_inline"

void ipsd_support_python_run_script(char *source, int pipefd[2]) {
    /* Use a virtual environment? */
    if (IPSD_PY_VENV[0] != '\0') {
        if (IPSD_PY_VENV_ISOLATED) {
            setenv("PYTHONHOME", IPSD_PY_VENV, 1);
        }

        setenv("PYTHONPATH", ISPD_PY_VENV_SITE, 1);
    }

    Py_Initialize();
    PyRun_SimpleString("import sys");

    if (IPSD_PY_VENV[0] == '\0') {
        PyRun_SimpleString("sys.path.insert(0, '" ISPD_PY_VENV_SITE "')");
    }

#if defined(IPSD_PLAT_APPLE)
    /* Just for safety. */
    setenv("OBJC_DISABLE_INITIALIZE_FORK_SAFETY", "YES", 1);
#endif /* IPSD_PLAT_APPLE */

    setenv("MPLBACKEND", "Agg", 1);
    PyRun_SimpleString(PYTHON_PRELUDE);

    /* Do the things we're here to do. */
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        ipsd_fatal("failed to dup2 pipe: %s", strerror(errno));
    }
    close(pipefd[1]);

    PyRun_SimpleString(source);

    /* Vitrify ourselves. */
    Py_Finalize();
}
