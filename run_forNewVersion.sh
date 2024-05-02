# ex: $1 -> acl1,  $2-> 100000
# sh run.sh acl1 90000
ulimit -s 81920
./classbench generate v4 ./vendor/parameter_files/$1_seed --count=$2 --db-generator=./vendor/db_generator/db_generator > $1_$2.txt
