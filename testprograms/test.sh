echo "Shell name: $0"
echo "Arg count: $#"
echo "Args: $*"
echo "First arg: $1"

USERNAME=pennywise

echo "User: $USERNAME"
echo "City: $CITY"

INPUTFILE=/home/pennywise/Desktop/untitled/input.txt
OUTPUTFILE=/home/pennywise/Desktop/untitled/output.txt

echo $INPUTFILE
echo $OUTPUTFILE


echo "alpha beta gamma delta epsilon"
| tr ' ' '\n'
| nl > $INPUTFILE

echo "Generated file:"
cat $INPUTFILE

echo "CPP files in cwd:"
ls *.cpp | wc -l

SPACED="hello   world"

echo $SPACED
echo "$SPACED"

COUNT=`ls $INPUTFILE | wc -l`
echo "Input file count: $COUNT"

ls does_not_exist | wc -l
echo "Pipeline exit status: $?"

cat $INPUTFILE
| grep a
| awk '{ print $2 }' > $OUTPUTFILE

echo "Matched:"
cat $OUTPUTFILE

MSG="Upper user: `echo USERNAME | tr a-z A-Z`"
echo "$MSG"

echo "FINAL" > $OUTPUTFILE
cat $OUTPUTFILE

exit 7