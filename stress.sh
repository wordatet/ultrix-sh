#!/home/mario/git/ULTRIX-src-4.2/bin/sh.d/sh

echo "Starting stress tests..."

# Test loops and variables
i=0
while [ $i -lt 100 ]
do
    VAR="Value_$i"
    if [ "$VAR" != "Value_$i" ]
    then
        echo "Variable mismatch at $i"
        exit 1
    fi
    i=`expr $i + 1`
done
echo "Loop test passed."

# Test pipes and redirects
echo "Piping test" | grep "Piping" > /tmp/sh_stress_tmp
RES="`cat /tmp/sh_stress_tmp`"
if [ "$RES" != "Piping test" ]
then
    echo "Pipe/Redirect test failed: expected 'Piping test', got '$RES'"
    exit 1
fi
rm /tmp/sh_stress_tmp
echo "Pipe/Redirect test passed."

# Test subshells and environment
TEST_ENV="Ultrix"; export TEST_ENV
(
    TEST_ENV="Subshell"; export TEST_ENV
    if [ "$TEST_ENV" != "Subshell" ]
    then
        exit 1
    fi
)
if [ "$TEST_ENV" != "Ultrix" ]
then
    echo "Subshell environment test failed"
    exit 1
fi
echo "Subshell/Environment test passed."

echo "All stress tests passed successfully!"
