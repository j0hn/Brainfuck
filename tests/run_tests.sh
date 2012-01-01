#!/bin/sh

failed() {
    TESTCASE=$1
    echo "Test failed in testcase: " $TESTCASE
}
success() {
    TESTCASE=$1
    echo "Test success in testcase: " $TESTCASE

}

clean() {
    rm hello_world_output 2> /dev/null
    rm hello_you_output 2> /dev/null
    rm ascii_chars_output 2> /dev/null
}

test_files(){
    FILE1=$1
    FILE2=$2
    TESTCASE=$3
    diff $FILE1 $FILE2 > /dev/null && success $TESTCASE || failed $TESTCASE
}

# AsciiChars test case
../brainfuck ascii_chars.bf > ascii_chars_output
test_files ascii_chars_output ascii_chars_expected_output "AsciiChars"

# HelloWorld test case
../brainfuck hello_world.bf > hello_world_output
test_files hello_world_output hello_world_expected_output "HelloWorld"

# HelloYou test case
echo "j0hn" | ../brainfuck hello_you.bf > hello_you_output
test_files hello_you_output hello_you_expected_output "HelloYou"

clean
