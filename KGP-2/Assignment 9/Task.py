import re
import sys
import requests
from ply.lex import lex
from ply.yacc import yacc
import codecs

def read_genre():
    file=open('rottentomatoes movie genre link.txt','r')
    genre_links={}
    count=0

    for line in file:
        if count > 0 :
            line=line.strip()
            if count%2 == 0 :
                genre_links[genre_types]=line
            else:
                genre_types = line[2:-1]
                genre_types = genre_types.strip()
            
        count += 1

    return genre_links

def read_genre_movies(genre_links):

    movies_list={}
    j = 1

    for i in genre_links:
        url = genre_links[i]
        # print(i, " : ", url)

        # if j % 2 == 1 :
        #     print(j,". ",i.ljust(20), end = "\t")
        # else :
        #     print(j, ". ", i.ljust(20))
        j+=1


        html_request= requests.get(url, allow_redirects=True)
        html_file=open("rottentomatoes.html",'wb')
        html_file.write(html_request.content)

        html_file=open('rottentomatoes.html','r').read()
        regex='(<td>\n*.*\n*\s*.*<\/a>)'
        genre_list=re.findall(regex,html_file)
        for html_lines in genre_list:
            movie_name=html_lines.split('\n')
            movie_name=(movie_name[2])[:-4].strip()
            #print(movie_name)
            link=re.search("/[a-z/_(0-9)-]*",html_lines)
            full_url="https://www.rottentomatoes.com"+link.group()
            movies_list[movie_name]=full_url

    return movies_list


def select_movie(movies_list):

    found = True

    while found :
        movie_name= input("Enter movie name with year : ")
        # movie_name = "Coco (2017)"

        for i in movies_list:
            if i != movie_name:
                continue

            link = movies_list[i]
            html_request= requests.get(link, allow_redirects=True)
            file_name="movie_1.html"
            html_file=open(file_name,'wb')
            html_file.write(html_request.content)
            # print("Your HTML file is sucessfully generated")
            return file_name

        print("Movie name not found. Try again.")
        

def movie_html() :

    genre_links = read_genre()

    movies_list = read_genre_movies(genre_links)

    # result = select_genre(genre_links)

    # movies_list = extract_movies()
    
    file_name = select_movie(movies_list)

    # print(file_name)
    
    return file_name


def movie_info_extractor(movie_link):

    tokens=('PRETITLE','POSTTITLE','PREDIRECTOR','PREPRODUCER','PREWRITER','PREBOXOFFICE','PRERUNTIME'
            ,'PRESTORYLINE','PRERECOMMEND','PREWATCH','HEADWATCH','WATCHEND','WATCHEND2','COST'
            ,'HEAD','MIDRECOMMEND','AEND','AEND2','MOVIEAEND','MOVIEAEND2','POSTRUNTIME','PRELANGUAGE', 'CASTHEAD'
            ,'CASTMID','CASTMID2','STRING')


    def t_PRETITLE(t):
        r'<title>'
        return t

    def t_POSTTITLE(t):
        r'<\/title>'
        return t

    def t_PREDIRECTOR(t):
        r'Director:<\/div>[*\s]+<div[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+'
        return t

    def t_PREPRODUCER(t):
        r'Producer:<\/div>[*\s]+<div[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+'
        return t

    def t_PREWRITER(t):
        r'Writer:<\/div>[*\s]+<div[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+'
        return t


    def t_CASTHEAD(t):
        r'<div[\s]class="cast-item[^>]+>[\s]+<[^>]+>[\s]+<[^>]+>[\s]+<[^>]+>[\s]+<\/a>[\s]+<[^>]+>[\s]+<[^>]+>[\s]+<[^y]+y\/'
        return t

    def t_CASTMID2(t):
        r'"[\s]class="unstyled[\s]articleLink"[\s]data-qa="cast-crew[^>]+>[\s]+<[^>]+>[\s]+'
        return t

    def t_CASTMID(t):
        r'[\s]+<\/span>[\s]+<\/a>[\s]+<span[\s][^>]+>[\s]+<br\/>[\s]+'
        return t


    def t_PREBOXOFFICE(t):
        r'Box[\s]Office[\s\](Gross[\]USA):<\/div>[\s]*<div[\s]class="meta-value"[\s]data-qa="movie-info-item-value">'
        return t

    def t_PRESTORYLINE(t):
        r'<div[\s]id="movieSynopsis"[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+'
        return t

    def t_PRERECOMMEND(t):
        r'You[\s]might[\s]also[\s]like<\/h2>[*\s]+<div[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+<tiles[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+<div[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+<[^m]+m\/'
        return t

    def t_MIDRECOMMEND(t):
        r'"[\s]class[^>]+>[\s]+<[^>]*>[*\s]+<[^>]*>[*\s]+<[^>]*>[*\s]+<[^>]*>[*\s]+<[^>]*>[*\s]+<[^>]*><[^>]*>[*\s]+<[^>]*><[^>]*>[*\s]+<[^>]*>'
        return t

    def t_PREWATCH(t):
        r'Where[\s]to[\s]watch<\/h2>[\s]+<[^>]+>[\s]+<[^>]+>[\s]+'
        return t

    def t_HEADWATCH(t):
        r'<[^>]+>[\s]+<[^d]+data-affiliate="'
        return t

    def t_WATCHEND2(t):
        r'"[\s]target="blank"[^>]+>[\s]+<[^>]+><[^>]+>[\s]+<[^>]+>[^<]+<\/p>[\s]+<[^>]+>[^<]+<\/p>[\s]+<\/a>[\s]+<\/li>[\s]+<\/ul>[\s]+'
        return t

    def t_WATCHEND(t):
        r'"[\s]target="blank"[^>]+>[\s]+<[^>]+><[^>]+>[\s]+<[^>]+>[^<]+<\/p>[\s]+<[^>]+>[^<]+<\/p>[\s]+<\/a>[\s]+<\/li>[\s]+'
        return t


    def t_PRERUNTIME(t):
        r'Runtime:<\/div>[*\s]+<div[\sa-zA-Z0-9=\:\-_\"]+>[*\s]+<time[\sa-zA-Z0-9=\"]+>'
        return t

    def t_POSTRUNTIME(t):
        r'[\s]*<\/time>'
        return t

    def t_PRELANGUAGE(t):
        r'Original[\s]Language:<\/div>[\s]*<div[\s]class="meta-value"[\s]data-qa="movie-info-item-value">'
        return t

    

    
        
    def t_HEAD(t):
        r'<a[\s]href[a-zA-Z0-9 =\/"_-]*>'
        return t

    def t_COST(t):
        r'\$[a-zA-Z0-9. ]+'
        return t


    def t_AEND2(t):
        r'<\/a>,[\n\s]*'
        return t

    def t_AEND(t):
        r'<\/a>[\n\s]*'
        return t

    def t_MOVIEAEND2(t):
        r'<\/span>[*\s]+<[^>]*>[*\s]+<[^>]*>[*\s]+<\/a>[\s]+<\/div>'
        return t

    def t_MOVIEAEND(t):
        r'<\/span>[*\s]+<[^>]*>[*\s]+<[^>]*>[*\s]+<\/a>[*\s]+<[^m]+m\/'
        return t

    def t_STRING(t):
        r'[a-zA-Z0-9().,:ÀÁÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýþÿ\-\'\_ ]+'
        return t

    


    def t_error(t):
        t.lexer.skip(1)

    

    def p_start(t):
        '''start : director 
                    | title
                    | producer
                    | writer
                    | cast
                    | boxoffice
                    | runtime
                    | storyline
                    | language
                    | recommend
                    | watch
        '''

    def p_director(t):
        'director : PREDIRECTOR name'
        movie_info['Director']=t[2].strip()

    def p_title(t):
        'title : PRETITLE STRING POSTTITLE'
        movie_info['Movie Name'] = t[2].strip()[slice(-18)] 

    def p_producer(t):
        ' producer : PREPRODUCER name'
        movie_info['Producer']=t[2].strip()
        
    def p_writer(t):
        'writer : PREWRITER name'
        movie_info['Writer']=t[2].strip()

    def p_cast(t):
        'cast : CASTHEAD STRING CASTMID2 STRING CASTMID STRING '
        star_name = t[4].strip() + " as " + t[6].strip() 
        movie_info['Cast'][star_name] = "https://www.rottentomatoes.com/celebrity/" + t[2].strip()


    def p_storyline(t):
        'storyline : PRESTORYLINE STRING'
        movie_info['Story line'] = t[2].strip()
        
    def p_boxoffice(t):
        'boxoffice : PREBOXOFFICE COST'
        movie_info['Box Office Collection']=t[2].strip()    
        
    def p_language(t):
        'language : PRELANGUAGE STRING'
        movie_info['Language']=t[2].strip()

    def p_recommendnames(t):
        'recommendname : MOVIEAEND STRING MIDRECOMMEND STRING MOVIEAEND2'
        movie_info['You Might Also Like'][t[4].strip()] = "https://www.rottentomatoes.com/m/" + t[2].strip()

    def p_recommendname(t):
        'recommendname : MOVIEAEND STRING MIDRECOMMEND STRING recommendname'
        movie_info['You Might Also Like'][t[4].strip()] = "https://www.rottentomatoes.com/m/" + t[2].strip()

    def p_recommend(t):
        'recommend : PRERECOMMEND STRING MIDRECOMMEND STRING recommendname'
        movie_info['You Might Also Like'][t[4].strip()] = "https://www.rottentomatoes.com/m/" + t[2].strip()

    


    def p_watch(t):
        'watch : PREWATCH watchname'
        movie_info['Where to Watch'] = t[2].strip()

    def p_watchname(t):
        'watchname : HEADWATCH STRING WATCHEND watchname'
        t[0] = t[2] + ", " + t[4]

    def p_watchnames(t):
        'watchname : HEADWATCH STRING WATCHEND2'
        t[0] = t[2]
        
    def p_runtime(t):
        'runtime : PRERUNTIME STRING POSTRUNTIME'
        movie_info['Runtime']=t[2].strip()

    def p_name(t):
        'name : HEAD STRING AEND'
        t[0]=t[2]
        
    def p_names(t):
        'name : HEAD STRING AEND2 name'
        t[0]=t[2]+", "+t[4]

    

    

    def p_error(t):
        pass


    page_info= codecs.open(movie_link, "r", "utf-8")

    lexer=lex()
    data=page_info.read()

    # lexer.input(data)
    # for tok in lexer:
    #     print(tok)

    movie_info={}
    movie_info['Cast'] = {}
    movie_info['You Might Also Like'] = {}

    parser = yacc()

    parser.parse(data)
    # print(movie_info['You Might Also Like'])

    return movie_info


def movie_information(movie_info, option):

    if option in movie_info:
        return movie_info[option]
    else :
        return "No information found\n"

def exit_the_prog():
    exit()


def movie_recommend(movie_info):
    i = 1
    links = []

    for movie_name in movie_info["You Might Also Like"]:
        print(i,". ", movie_name)
        links.append(movie_info["You Might Also Like"][movie_name])

        i += 1

    choice = int(input("\nSelect movie no to view info, select 0 to return: "))

    if choice == 0:
        return

    elif choice < 6 :
        html_request= requests.get(links[choice - 1], allow_redirects=True)
        file_name="movie_1.html"
        html_file=open(file_name,'wb')
        html_file.write(html_request.content)
        prog_parser(file_name)
    


def movie_cast(movie_info) :
    i = 1
    links = []

    for cast_name in movie_info["Cast"]:
        print(i,". ", cast_name)
        links.append(movie_info["Cast"][cast_name])

        i += 1

    choice = int(input("\nSelect Cast no to view info, select 0 to return: "))

    if choice == 0:
        return

    elif choice < 16 :
        html_request= requests.get(links[choice - 1], allow_redirects=True)
        file_name="movie_1.html"
        html_file=open(file_name,'wb')
        html_file.write(html_request.content)
        print(file_name)
        return
        # prog_cast_parser(file_name)



def get_movie_info(option, movie_info):
    
    if option == 1:
        return movie_information(movie_info, "Director")
    elif option == 2:
        return movie_information(movie_info, "Writer")
    elif option == 3:
        return movie_information(movie_info, "Producer")
    elif option == 4:
        return movie_information(movie_info, "Language")
    elif option == 5:
        return movie_cast(movie_info)
    elif option == 6:
        return movie_information(movie_info, "Story line")
    elif option == 7:
        return movie_information(movie_info, "Box Office Collection")
    elif option == 8:
        return movie_information(movie_info, "Runtime")
    elif option == 9:
        return movie_recommend(movie_info)
    elif option == 10:
        return movie_information(movie_info, "Where to Watch")
    elif option == 99:
        print("\n\nThank You\n\n")
        exit()
    else:
        return "Invalid Option"
    
    


def info_display(movie_info):

    choice = 1

    while(choice):
        print("Movie Name : ",movie_info['Movie Name'])
        print("1. Director(s)".ljust(25), "\t", "2. Writers".ljust(20))
        print("3. Producer(s)".ljust(25), "\t", "4. Language".ljust(20))
        print("5. Cast & Crew".ljust(25), "\t", "6. Storyline".ljust(20))
        print("7. Box Office Collection".ljust(25), "\t", "8. Runtime".ljust(20))
        print("9. Recommend Movie".ljust(25), "\t", "10. Where to Watch".ljust(20))
        print("99. Exit Program")

        option = int(input("Select option no to display info(1 or 5): "))
        # option = 9

        print("\n", get_movie_info(option, movie_info), "\n")


def prog_parser(file_name):

    movie_link = file_name

    movie_info = movie_info_extractor(movie_link)
    
    # for element in movie_info :
    #     print(element, " : ", movie_info[element])

    info_display(movie_info)



# Main Function
if __name__ == "__main__":

    file_name = movie_html()

    prog_parser(file_name)