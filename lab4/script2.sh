#!/bin/bash

main_files=""
module_files=""
other_files=""

for file in `find $1 -type f -name '*.c'`; do
    file_path=`readlink -f $file`

    # Check for the main function
    if grep -q 'int main *(.*)' $file; then
        printf_count=`grep -wc 'printf' $file`
        fprintf_count=`grep -wc 'fprintf' $file`
        main_files+="$file_path: $printf_count, $fprintf_count\n"

    # Check for the init_module function
    elif grep -q 'int init_module *(.*)' $file; then
        printk_lines=`grep -wn 'printk' $file | sed -e 's/:.*//' | tr '\n' ','`
        module_files+="$file_path: ${printk_lines}\n"
    else
        other_files+="$file_path\n"
    fi
done

# Output files with a main function
if [ -n "$main_files" ]; then
    echo "Main Files:"
    printf "$main_files"
else
    echo "No main file"
fi

# Output files with the init_module function
if [ -n "$module_files" ]; then
    printf "\nModule Files:\n"
    printf "$module_files"
else
    printf "\nNo module file"
fi

# Output other source files
if [ -n "$other_files" ]; then
    printf "\nOther Files:\n"
    printf "$other_files"
else
    printf "\nNo other file"
fi
