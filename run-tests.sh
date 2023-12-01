#!/bin/bash

COMMAND="./multithread_lzw"

echo "Run command: ${COMMAND}"

FAIL=0

function run_test_sync {
    # run 
    ${COMMAND} 0 tests/"$1".in tests/sync_"$1".out
    # check answer
    diff tests/sync_"$1".out tests/"$1".in
    if [ $? -ne 0 ]; then
        FAIL=1
        echo -e "\033[31mSYNC FAIL\033[0m"
    else
        echo -e "\033[32mSYNC PASS\033[0m"
    fi
    rm -f tests/sync_"$1".out
}

function run_test_async {
    # run 
    ${COMMAND} 1 tests/"$1".in tests/async_"$1".out
    # check answer
    diff tests/async_"$1".out tests/"$1".in
    if [ $? -ne 0 ]; then
        FAIL=1
        echo -e "\033[31mASYNC FAIL\033[0m"
    else
        echo -e "\033[32mASYNC PASS\033[0m"
    fi
    rm -f tests/async_"$1".out
}

for test_name in $(cat tests.txt); do
    test_name=$(echo $test_name)  # To remove \r from tests.txt on Windows
    echo ========= $test_name =========
    run_test_sync "${test_name}"
    run_test_async "${test_name}"
done

if [[ "$FAIL" == "0" ]]; then
    echo -e "=========  \033[32;1mALL PASS\033[0m ========= "
else
    echo -e "=========  \033[31;1mSOME FAIL\033[0m ========= "
fi

exit $FAIL
