#!/bin/bash

# touch namelist
# Then add some names
sort -r -n namelist > namelist.sorted
echo \'namelist\' was sorted in reverse order. Sorted list was saved to \'namelist.sorted\'
echo \'namelist.sorted\' first ten entries:
head namelist.sorted -n 10
