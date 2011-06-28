#!/bin/bash
version="0.0.1"

echoc () { 
  echo -e "\E["$1"m$2\E[0m";
}

catall () { 
    echo    "##################################" >> $2;
    echo    "### start of $1" >> $2;
    echo -e "##################################\n" >> $2;
    cat $1 >> $2;
    echo -e "### end of $1\n" >> $2;  
}



echoc 31 "catall $version... by viralex (alessandro.rosetti@gmail.com)";
echoc 30 "usage: $0 <absolute path without /> <output pdf name>";
echoc 30 "example: \"$0 /home/viralex project\" , yes I am lazy";

rm "$1/$2" "$1/$2.pdf"

dlist=$(ls -d */)
for dir in $dlist ; do

       echoc 32 "scanning $dir";
	   cd $dir;
       flist=$(ls -f *)
       for file in $flist ; do
          	if [[ "$dir" != ".." ]]; then
            if [[ "$dir" != "." ]]; then
                echoc 30 "  adding $file";
                catall $file "$1/$2";
            fi
            fi
       done
       cd ..;
done
cd -

echoc 32 "making pdf $2.pdf";
cd $1
text2pdf -s 9 -o "$2.pdf" $2
