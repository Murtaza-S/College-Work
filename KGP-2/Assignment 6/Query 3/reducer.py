import sys

#Function to find minimum key : value pair
def list_top_users():
    file = open("result.txt")
    user_list = []

    for line in file:
        user_list.append(int(line))

    print(user_list)
    return user_list


current_user = -1
count = 0
user_count = 0

top_users = list_top_users()
tracked_users = []


for line in sys.stdin:
    users = line.split(" ")
    users[0] = int(users[0])
    users[1] = int(users[1])

    if users[0] in top_users:
        if users[1] not in top_users and users[1] not in tracked_users:
            tracked_users.append(users[1])

    elif users[1] in top_users:
        if users[0] not in top_users and users[0] not in tracked_users:
            tracked_users.append(users[0])

for _ in tracked_users:
    print(_)