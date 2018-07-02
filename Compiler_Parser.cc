#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

struct sets
{
    std::string str;
    std::vector<string> prev;
    std::vector<string> cur;
};

std::vector<string> nonvec;
std::vector<string> tvec;
std::vector<string> termvec;

std::vector<struct sets> first;
std::vector<struct sets> follow;

int terminal_length;
int h,dh;
int token_length;
int line_no = 1;
char token[255]; // token string

int syntax_error()
{
    cout<<"Error, syntax is wrong\n";
    return 0;
}

int skip_space()
{
    char c;

    c = getchar();
    line_no += (c == '\n');
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
        line_no += (c == '\n');
    }
    ungetc(c, stdin);
    return 0;
}

int scan_nonterminal()
{
    char c;
    token_length = 0;
    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c))
        {
            token[token_length] = c;
            token_length++;
            c = getchar();
        }
        if (!feof(stdin))
        {
            ungetc(c, stdin);
        }
        token[token_length] = '\0';
        return 0;
    }
    else
    {
        return 1;
    }
}

int scan()
{
    char c;
    token_length = 0;
    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c))
        {
            token[token_length] = c;
            token_length++;
            c = getchar();
        }
        if (!feof(stdin))
        {
            ungetc(c, stdin);
        }
        token[token_length] = '\0';
        return 0;
    }
    else
    {
        return 1;
    }
}

int counter()
{
    std::vector<string>::iterator t;
    std::vector<string>::iterator term;
    int c = 0;
    int count = 0;

    for(term = termvec.begin(); term != termvec.end(); ++term)
    {
        count = 0;
        for(t = tvec.begin(); t != tvec.end(); ++t)
        {
            c=0;
            while( (*t).compare("#") !=0 && (*t).compare("##") !=0)
            {
                if( (*term).compare((*t)) == 0 )
                {
                    c=1;
                }
                t++;
            }
            count += c;
        }
        cout<<(*term)<<": "<<count<<endl;
    }
    return 0;
}

int scanright()
{
    int i;
    std::vector<string>::iterator t;
    std::vector<string>::iterator n;

    for(t = tvec.begin(); t != tvec.end(); ++t)
    {
        if( (*t).compare("#") == 0 || (*t).compare("##") == 0)
        {
            continue;
        }
        i = 0;
        for(n = nonvec.begin(); n != nonvec.end(); ++n)
        {
            if( (*t).compare(*n) == 0)
            {
                i = 1;
                break;
            }
        }
        if (i == 0)
        {
            if(std::find( termvec.begin(), termvec.end(), (*t)) != termvec.end())
            {
            }
            else
            {
                termvec.push_back(*t);
            }
        }
    }
    return 0;
}

int display_task0()
{
    std::sort (termvec.begin(), termvec.end());
    for(std::vector<string>::iterator it = nonvec.begin(); it != nonvec.end(); ++it)
    {
        cout<<(*it)<<" ";
    }
    cout<<endl;
    counter();
    return 0;
}

int caltask0()
{
    char c;
    h = 0;
    dh = 0;

//scaning non terminal
    while(!h)
    {
        skip_space();
        c = getchar();
        if (c == '#')
        {
            h = 1;
            break;
        }
        else
        {
            ungetc(c, stdin);
        }
        if(!scan_nonterminal())
        {
            nonvec.push_back(std::string(token));
        }
    }

//scanning terminal
    while(!dh)
    {
        skip_space();
        c = getchar();
        if (c == '#')
        {
            c = getchar();
            if (c == '#')
            {
                tvec.push_back(std::string("##"));
                dh = 1;
                break;
            }
            else
            {
                ungetc(c, stdin);
                tvec.push_back(std::string("#"));
            }
        }
        else
        {
            ungetc(c, stdin);
        }
        if(!scan())
        {
            tvec.push_back(std::string(token));
        }
    }
    scanright();
    return 0;
}

int task0()
{
    caltask0();
    //display_task0();
    for(std::vector<string>::iterator it = nonvec.begin(); it != nonvec.end(); ++it)
    {
        cout<<(*it)<<" ";
    }
    cout<<endl;
    for(std::vector<string>::iterator it = termvec.begin(); it != termvec.end(); ++it)
    {
        cout<<(*it)<<" ";
    }
    cout<<endl;
    return 0;
}

int display_task1()
{
    int c;
    std::vector<struct sets>::iterator f;
    std::vector<std::string>::iterator f1;
    for(f = first.begin(); f != first.end(); ++f)
    {
        c = 1;
        std::sort ( ((*f).cur).begin(), ((*f).cur).end());
        cout<<"FIRST("<<(*f).str<<") = { ";
        for(f1 = ((*f).cur).begin(); f1 != ((*f).cur).end(); ++f1)
        {
            cout<<(*f1);
            if(c < ((*f).cur).size())
               cout<<", ";
            c++;
        }
        cout<<" }"<<endl;
    }
    return 0;
}

int task1()
{
    int start = 1;
    int counter = 0;
    int has_epsilon = 0;
    int add_epsilon = 0;
    struct sets myset;
    std::vector<string>::iterator t;
    std::vector<string>::iterator n;
    std::vector<string>::iterator s;
    std::vector<struct sets>::iterator f;
    std::vector<struct sets>::iterator f1;

    caltask0();
    for(n = nonvec.begin(); n!= nonvec.end(); ++n)
    {
        myset.str = (*n);
        first.push_back(myset);
    }
    while(start)
    {
        counter ++;
        for(t = tvec.begin(); t != tvec.end(); ++t)
        {
            add_epsilon = 0;
            if( (*t).compare("#") != 0 && (*t).compare("##") != 0)
            {
                for(f = first.begin(); f!= first.end(); ++f)
                {
                    if( ((*f).str).compare((*t)) == 0 )
                    {
                        break;
                    }
                }
                t++;
                //Checking for epsilon
                if( (*t).compare("#") == 0)
                {
                    if(!(std::find( ((*f).cur).begin(), ((*f).cur).end(), "#") != ((*f).cur).end()))
                    {
                        ((*f).cur).push_back("#");
                    }
                }
                while( (*t).compare("#") != 0 && (*t).compare("##") != 0)
                {
                    //terminal check
                    if(std::find( termvec.begin(), termvec.end(), *t) != termvec.end())
                    {
                        add_epsilon = 0;
                        if(!(std::find( ((*f).cur).begin(), ((*f).cur).end(), *t) != ((*f).cur).end()))
                        {
                            ((*f).cur).push_back(*t);
                        }
                        while( (*t).compare("#") != 0 && (*t).compare("##") != 0)
                        {
                            t++;
                        }
                        break;
                    }
                    //non terminal check
                    else
                    {
                        has_epsilon = 0;
                        for(f1 = first.begin(); f1 != first.end(); ++f1)
                        {
                            if( ((*f1).str).compare(*t) == 0 )
                            {
                                break;
                            }
                        }
                        //comparing the first sets to add the first set
                        for( s = ((*f1).prev).begin(); s != ((*f1).prev).end(); ++s)
                        {
                            //Adding first(B) - epsilon
                            if ( (*s).compare("#") != 0)
                            {
                                if(!(std::find( ((*f).cur).begin(), ((*f).cur).end(), *s) != ((*f).cur).end()))
                                {
                                    ((*f).cur).push_back(*s);
                                }
                            }
                            else
                            {
                                add_epsilon = 1;
                                has_epsilon = 1;
                            }
                        }
                        //need to check for epsilon
                        if (has_epsilon == 0)
                        {
                            add_epsilon = 0;
                            while( (*t).compare("#") != 0 && (*t).compare("##") != 0)
                            {
                                t++;
                            }
                            break;
                        }
                    }
                    t++;
                    if( (*t).compare("#") == 0)
                    {
                        if (add_epsilon == 1)
                        {
                            if(!(std::find( ((*f).cur).begin(), ((*f).cur).end(), "#") != ((*f).cur).end()))
                            {
                                ((*f).cur).push_back("#");
                            }
                        }
                    }
                }
            }
        }
        int flag = 0;
        for(f = first.begin(); f!= first.end(); ++f)
        {
            if ( ((*f).cur).size() == ((*f).prev).size())
            {
                std::sort ( ((*f).cur).begin(), ((*f).cur).end() );
                std::sort ( ((*f).prev).begin(), ((*f).prev).end() );
                if (!(std::equal( ((*f).cur).begin(), ((*f).cur).end(), ((*f).prev).begin() )))
                {
                    flag = 1;
                    break;
                }
            }
            else
            {
                flag = 1;
                break;
            }
        }
        if((flag == 0) && (counter != 1))
        {
            start = 0;
        }
        for(f = first.begin(); f!= first.end(); ++f)
        {
            ((*f).prev).erase( ((*f).prev).begin(), ((*f).prev).end() );
            ((*f).prev).reserve(((*f).cur).size());
            for(s = ((*f).cur).begin(); s != ((*f).cur).end(); ++s)
            {
                ((*f).prev).push_back(*s);
            }
        }
    }
    return 0;
}

int display_task2()
{
    int c;
    std::vector<struct sets>::iterator f;
    std::vector<std::string>::iterator f1;
    for(f = follow.begin(); f != follow.end(); ++f)
    {
        c = 1;
        std::sort ( ((*f).cur).begin(), ((*f).cur).end());
        cout<<"FOLLOW("<<(*f).str<<") = { ";
        for(f1 = ((*f).cur).begin(); f1 != ((*f).cur).end(); ++f1)
        {
            cout<<(*f1);
            if(c < ((*f).cur).size())
               cout<<", ";
            c++;
        }
        cout<<" }"<<endl;
    }
    return 0;
}

int task2()
{
    int start = 1;
    int counter = 0;
    int has_epsilon = 0;
    int add_epsilon = 0;
    struct sets myset;
    std::vector<string>::iterator t, k;
    std::vector<string>::iterator n;
    std::vector<string>::iterator s;
    std::vector<struct sets>::iterator f;
    std::vector<struct sets>::iterator f1;
    std::vector<struct sets>::iterator fo;

    task1();
    for(n = nonvec.begin(); n!= nonvec.end(); ++n)
    {
        myset.str = (*n);
        follow.push_back(myset);
    }
    //adding $ to the first non_terminal
    (follow[0].cur).push_back("$");
    while(start)
    {
        counter ++;
        for(fo = follow.begin(); fo!= follow.end(); ++fo)
        {
            for(t = tvec.begin(); t != tvec.end(); ++t)
            {
                if( (*t).compare("#") != 0 && (*t).compare("##") != 0)
                {
                    //f is B
                    for(f = follow.begin(); f!= follow.end(); ++f)
                    {
                        if( ((*f).str).compare((*t)) == 0 )
                        {
                            break;
                        }
                    }
                    t++;
                    while( (*t).compare("#") != 0 && (*t).compare("##") != 0)
                    {
                        if( ((*fo).str).compare(*t) == 0 )
                        {
                            t++;
                            //checking for terminal
                            if(std::find( termvec.begin(), termvec.end(), *t) != termvec.end())
                            {
                                if(!(std::find( ((*fo).cur).begin(), ((*fo).cur).end(), *t) != ((*fo).cur).end()))
                                {
                                    ((*fo).cur).push_back(*t);
                                }
                            }
                            //adding first of C0 to follow of A
                            has_epsilon = 0;
                            add_epsilon = 0;
                            else if( std::find( nonvec.begin(), nonvec.end(), *t) != nonvec.end() )
                            {
                                for( f1 = first.begin(); f1 != first.end(); ++f1)
                                {
                                    if( ((*f1).str).compare(*t) == 0 )
                                    {
                                        for( s = ((*f1).cur).begin(); s != ((*f1).cur).end(); ++s)
                                        {
                                            if ( (*s).compare("#") != 0)
                                            {
                                                if(!(std::find( ((*fo).cur).begin(), ((*fo).cur).end(), *s) != ((*fo).cur).end()))
                                                {
                                                    ((*fo).cur).push_back(*s);
                                                }
                                            }
                                            else
                                            {
                                                has_epsilon = 1;
                                            }
                                        }
                                        break;
                                    }
                                }
                                if(has_epsilon == 1)
                                {
                                    k = t;
                                    k++;
                                    add_epsilon = 1;
                                    while( (*k).compare("#") != 0 && (*k).compare("##") != 0 && has_epsilon == 1)
                                    {
                                        if( std::find( nonvec.begin(), nonvec.end(), *k) != nonvec.end() )
                                        {
                                            for( f1 = first.begin(); f1 != first.end(); ++f1)
                                            {
                                                if( ((*f1).str).compare(*k) == 0 )
                                                {
                                                    has_epsilon = 0;
                                                    for( s = ((*f1).cur).begin(); s != ((*f1).cur).end(); ++s)
                                                    {
                                                        if ( (*s).compare("#") != 0)
                                                        {
                                                            if(!(std::find( ((*fo).cur).begin(), ((*fo).cur).end(), *s) != ((*fo).cur).end()))
                                                            {
                                                                ((*fo).cur).push_back(*s);
                                                            }
                                                        }
                                                        else
                                                        {
                                                            has_epsilon = 1;
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                            k++;
                                        }
                                        else
                                        {
                                            if(std::find( termvec.begin(), termvec.end(), *k) != termvec.end())
                                            {
                                                if(!(std::find( ((*fo).cur).begin(), ((*fo).cur).end(), *k) != ((*fo).cur).end()))
                                                {
                                                    ((*fo).cur).push_back(*k);
                                                }
                                            }
                                            add_epsilon = 0;
                                            break;
                                        }
                                    }
                                    if(add_epsilon == 1 && has_epsilon == 1)
                                    {
                                        for( s = ((*f).cur).begin(); s != ((*f).cur).end(); ++s)
                                        {
                                            if(!(std::find( ((*fo).cur).begin(), ((*fo).cur).end(), *s) != ((*fo).cur).end()))
                                            {
                                                ((*fo).cur).push_back(*s);
                                            }
                                        }
                                    }
                                }
                            }
                            //adding follow of B to follow of A
                            else if( (*t).compare("#") == 0)
                            {
                                for( s = ((*f).cur).begin(); s != ((*f).cur).end(); ++s)
                                {
                                    if(!(std::find( ((*fo).cur).begin(), ((*fo).cur).end(), *s) != ((*fo).cur).end()))
                                    {
                                        ((*fo).cur).push_back(*s);
                                    }
                                }
                                break;
                            }
                            t--;
                        }
                        t++;
                    }
                }
            }
        }
        int flag = 0;
        for(f = follow.begin(); f!= follow.end(); ++f)
        {
            if ( ((*f).cur).size() == ((*f).prev).size())
            {
                std::sort ( ((*f).cur).begin(), ((*f).cur).end() );
                std::sort ( ((*f).prev).begin(), ((*f).prev).end() );
                if (!(std::equal( ((*f).cur).begin(), ((*f).cur).end(), ((*f).prev).begin() )))
                {
                    flag = 1;
                    break;
                }
            }
            else
            {
                flag = 1;
                break;
            }
        }
        if((flag == 0) && (counter != 1))
        {
            start = 0;
        }
        for(f = follow.begin(); f!= follow.end(); ++f)
        {
            ((*f).prev).erase( ((*f).prev).begin(), ((*f).prev).end() );
            ((*f).prev).reserve(((*f).cur).size());
            for(s = ((*f).cur).begin(); s != ((*f).cur).end(); ++s)
            {
                ((*f).prev).push_back(*s);
            }
        }
    }
    return 0;
}


int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        printf("Error: missing argument\n");
        return 1;
    }
    task = atoi(argv[1]);
    switch (task) {
        case 0:
            task0();
            break;

        case 1:
            task1();
            display_task1();
            break;

        case 2:
            task2();
            display_task2();
            break;

        default:
            printf("Error: unrecognized task number %d\n", task);
            break;
    }
    return 0;
}
