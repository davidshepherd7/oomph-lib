#!/bin/sh

variable_from_makefile()
{
    echo 'print-var:; @echo $('$1')' | make -f - -f Makefile print-var
}

check_number_of_oks()
{
    if [ "$#" -ne 2 ]; then
        echo "check number of oks requires exactly two inputs:"
        echo "* The number of OKs expected"
        echo "* The path to the validation.log file."
    fi

    if ! [ -f "$2" ]; then
        echo "The second argument must be a path to the validation.log file."
    fi

    NUM_TESTS=$1

# Get the number of Oks
    OK_COUNT=$(grep -c 'OK' $2)

# Compare with Number of tests and print a message
    if [ $OK_COUNT -eq $NUM_TESTS ]; then
        echo " "
        echo "======================================================================"
        echo " " 
        echo "All tests in" 
        echo " " 
        echo "    `pwd`    "
        echo " "
        echo "passed successfully."
        echo " "
        echo "======================================================================"
        echo " " 
        exit 0
    else
        if [ $OK_COUNT -lt $NUM_TESTS ]; then
            echo " "
            echo "======================================================================"
            echo " " 
            echo "Only $OK_COUNT of $NUM_TESTS test(s) passed; see"
            echo " " 
            echo "    `pwd`/Validation/validation.log"
            echo " " 
            echo "for details" 
            echo " " 
            echo "======================================================================"
            echo " "
            exit 1
        else 
            echo " "
            echo "======================================================================"
            echo " " 
            echo "More OKs than tests! Need to update NUM_TESTS in"
            echo " " 
            echo "    `pwd`/validate.sh"
            echo " "
            echo "======================================================================"
            echo " "
            exit 2
        fi
    fi
# Never get here
    exit 10
}

safe_fp_diff()
{
    if $3; then
        echo "dummy [OK] -- Can't run fpdiff.py because we don't have python or validata" 
    else
        ${OOMPH_ROOT}/bin/fpdiff.py $1 $2
    fi
}

print_name_info()
{
    echo
    echo "$1"
    echo "--------------------------"
    echo
    echo "Validation directory: "
    echo
    echo "   $(pwd)"
    echo
}

do_everything()
{

    oomph_root=$(variable_from_makefile abs_top_srcdir)

    print_name_info "1D demo poisson validation" >> Validation/validation.log

    safe_fp_diff $1 $2 $3 >> Validation/validation.log

# Append output to global validation log file
    cat Validation/validation.log >> $oomph_root/validation.log

# Check that we got the correct number of OKs in validation.log
    check_number_of_oks $NUM_TESTS Validation/validation.log
}
