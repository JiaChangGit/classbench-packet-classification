# ex: $1 -> acl1,  $2-> 100000
# sh run.sh acl1 100000
ulimit -s 81920
./db_generator -bc ../parameter_files/$1_seed $2 2 0.5 -0.1 $1_$2.txt
