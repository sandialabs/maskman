#!/usr/bin/env bash

set -e
cd "${SLURM_SUBMIT_DIR}"
make $*
code_err=$?

exit ${code_err}
