import sys
fp = open(sys.argv[1], ‘r’ )

#user_map = []


for line in fp : 

    item = eval (line)

    if item ['id'] < 5 :
        print(item['id'] , " : ", item['categories'])
    

