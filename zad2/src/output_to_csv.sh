#!/bin/bash

for f in ./output_*.m; do
    echo "Converting $f"
    f_csv="${f%.*}.csv"
    echo "Matrix dimensions: m = n, GFLOPS" > $f_csv
    cat $f | tail -n +3 | sed '$d' | sed -r 's/ /,/g' >> $f_csv
    echo "Saved as $f_csv"
done
