#! /bin/sh

# Get functions from the validate_functions.sh file
. ${OOMPH_ROOT}/bin/validate_functions.sh

# See if we have an input arg before we set the "nounset" option.
no_fp_diff=false
if test "$1" = "no_fpdiff"; then
    no_fp_diff=true
fi

# Tell bash to fail immediately on errors or if variables are not set (this
# stops it doing really stupid things like trying to create files in / if
# oomph_root isn't set).
set -o errexit
set -o nounset


#Set the number of tests to be checked
NUM_TESTS=1


# Setup validation directory
#---------------------------
touch Validation
rm -r -f Validation
mkdir Validation

# Validation for demo poisson
#----------------------------
cd Validation

echo "Running 1D demo poisson validation "
mkdir RESLT
cd RESLT
../../one_d_poisson > ../OUTPUT_one_d_poisson
cd ..
echo "done"

# Compare results (only if we have python + validata
cat RESLT/soln0.dat RESLT/soln1.dat > one_d_poisson_results.dat

cd ..

do_everything validata/one_d_poisson_results.dat.gz \
    Validation/one_d_poisson_results.dat $no_fp_diff $OOMPH_ROOT
