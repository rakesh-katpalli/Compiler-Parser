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
    std::vector<string> previous_sets;
    std::vector<string> current_sets;
};

std::vector<string> non_terminal_vector;
std::vector<string> token_vector;
std::vector<string> terminal_vector;

std::vector<struct sets> first;
std::vector<struct sets> follow;

int terminal_length;
int hash,double_hash;
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
    char ch;

    ch = getchar();
    line_no += (ch == '\n');
    while (!feof(stdin) && isspace(ch))
    {
        ch = getchar();
        line_no += (ch == '\n');
    }
    ungetc(ch, stdin);
    return 0;
}

int scan_non_terminal()
{
    char ch;
    token_length = 0;
    ch = getchar();
    if (isalpha(ch))
    {
        while (isalnum(ch))
        {
            token[token_length] = ch;
            token_length++;
            ch = getchar();
        }
        if (!feof(stdin))
        {
            ungetc(ch, stdin);
        }
        token[token_length] = '\0';
        return 0;
    }
    else
    {
        return 1;
    }
}

int scanner()
{
    char ch;
    token_length = 0;
    ch = getchar();
    if (isalpha(ch))
    {
        while (isalnum(ch))
        {
            token[token_length] = ch;
            token_length++;
            ch = getchar();
        }
        if (!feof(stdin))
        {
            ungetc(ch, stdin);
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
    std::vector<string>::iterator iterator1;
    std::vector<string>::iterator iterator2;
    int ch = 0;
    int count = 0;

    for(iterator1 = terminal_vector.begin(); iterator1 != terminal_vector.end(); ++iterator1)
    {
        count = 0;
        for(iterator2 = token_vector.begin(); iterator2 != token_vector.end(); ++iterator2)
        {
            ch=0;
            while( (*iterator2).compare("#") !=0 && (*iterator2).compare("##") !=0)
            {
                if( (*iterator1).compare((*iterator2)) == 0 )
                {
                    ch=1;
                }
                iterator2++;
            }
            count += ch;
        }
        cout<<(*iterator1)<<": "<<count<<endl;
    }
    return 0;
}

int scan_right()
{
    int i;
    std::vector<string>::iterator iterator1;
    std::vector<string>::iterator iterator2;

    for(iterator1 = token_vector.begin(); iterator1 != token_vector.end(); ++iterator1)
    {
        if( (*iterator1).compare("#") == 0 || (*iterator1).compare("##") == 0)
        {
            continue;
        }
        i = 0;
        for(iterator2 = non_terminal_vector.begin(); iterator2 != non_terminal_vector.end(); ++iterator2)
        {
            if( (*iterator1).compare(*iterator2) == 0)
            {
                i = 1;
                break;
            }
        }
        if (i == 0)
        {
            if(!(std::find( terminal_vector.begin(), terminal_vector.end(), (*iterator1)) != terminal_vector.end()))
            {
                terminal_vector.push_back(*iterator1);
            }    
        }
    }
    return 0;
}

int display_task0()
{
    for(std::vector<string>::iterator iterator = non_terminal_vector.begin(); iterator != non_terminal_vector.end(); ++iterator)
    {
        cout<<(*iterator)<<" ";
    }
    cout<<endl;
    for(std::vector<string>::iterator iterator = terminal_vector.begin(); iterator != terminal_vector.end(); ++iterator)
    {
        cout<<(*iterator)<<" ";
    }
    cout<<endl;
    return 0;
}

int task0()
{
    char ch;
    hash = 0;
    double_hash = 0;

//scaning non terminal
    while(!hash)
    {
        skip_space();
        ch = getchar();
        if (ch == '#')
        {
            hash = 1;
            break;
        }
        else
        {
            ungetc(ch, stdin);
        }
        if(!scan_non_terminal())
        {
            non_terminal_vector.push_back(std::string(token));
        }
    }

//scanning terminal
    while(!double_hash)
    {
        skip_space();
        ch = getchar();
        if (ch == '#')
        {
            ch = getchar();
            if (ch == '#')
            {
                token_vector.push_back(std::string("##"));
                double_hash = 1;
                break;
            }
            else
            {
                ungetc(ch, stdin);
                token_vector.push_back(std::string("#"));
            }
        }
        else
        {
            ungetc(ch, stdin);
        }
        if(!scanner())
        {
            token_vector.push_back(std::string(token));
        }
    }
    scan_right();
    return 0;
}

int display_task1()
{
    int counter;
    std::vector<struct sets>::iterator iterator1;
    std::vector<std::string>::iterator iterator2;
    for(iterator1 = first.begin(); iterator1 != first.end(); ++iterator1)
    {
        counter = 1;
        std::sort ( ((*iterator1).current_sets).begin(), ((*iterator1).current_sets).end());
        cout<<"FIRST("<<(*iterator1).str<<") = { ";
        for(iterator2 = ((*iterator1).current_sets).begin(); iterator2 != ((*iterator1).current_sets).end(); ++iterator2)
        {
            cout<<(*iterator2);
            if(counter < ((*iterator1).current_sets).size())
               cout<<", ";
            ccounter++;
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
    std::vector<string>::iterator iterator1;
    std::vector<string>::iterator iterator2;
    std::vector<string>::iterator iterator3;
    std::vector<struct sets>::iterator set_iterator1;
    std::vector<struct sets>::iterator set_iterator2;

    task0();
    for(iterator2 = non_terminal_vector.begin(); iterator2!= non_terminal_vector.end(); ++iterator2)
    {
        myset.str = (*iterator2);
        first.push_back(myset);
    }
    while(start)
    {
        counter ++;
        for(iterator1 = token_vector.begin(); iterator1 != token_vector.end(); ++iterator1)
        {
            add_epsilon = 0;
            if( (*iterator1).compare("#") != 0 && (*iterator1).compare("##") != 0)
            {
                for(set_iterator1 = first.begin(); set_iterator1!= first.end(); ++set_iterator1)
                {
                    if( ((*set_iterator1).str).compare((*iterator1)) == 0 )
                    {
                        break;
                    }
                }
                iterator1++;
                //Checking for epsilon
                if( (*iterator1).compare("#") == 0)
                {
                    if(!(std::find( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end(), "#") != ((*set_iterator1).current_sets).end()))
                    {
                        ((*set_iterator1).current_sets).push_back("#");
                    }
                }
                while( (*iterator1).compare("#") != 0 && (*iterator1).compare("##") != 0)
                {
                    //terminal check
                    if(std::find( terminal_vector.begin(), terminal_vector.end(), *iterator1) != terminal_vector.end())
                    {
                        add_epsilon = 0;
                        if(!(std::find( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end(), *iterator1) != ((*set_iterator1).current_sets).end()))
                        {
                            ((*set_iterator1).current_sets).push_back(*iterator1);
                        }
                        while( (*iterator1).compare("#") != 0 && (*iterator1).compare("##") != 0)
                        {
                            iterator1++;
                        }
                        break;
                    }
                    //non terminal check
                    else
                    {
                        has_epsilon = 0;
                        for(set_iterator2 = first.begin(); set_iterator2 != first.end(); ++set_iterator2)
                        {
                            if( ((*set_iterator2).str).compare(*iterator1) == 0 )
                            {
                                break;
                            }
                        }
                        //comparing with the previous_sets first sets to add the current_sets first set
                        for( iterator3 = ((*set_iterator2).previous_sets).begin(); iterator3 != ((*set_iterator2).previous_sets).end(); ++iterator3)
                        {
                            //Adding first(B) - epsilon
                            if ( (*iterator3).compare("#") != 0)
                            {
                                if(!(std::find( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end(), *iterator3) != ((*set_iterator1).current_sets).end()))
                                {
                                    ((*set_iterator1).current_sets).push_back(*iterator3);
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
                            while( (*iterator1).compare("#") != 0 && (*iterator1).compare("##") != 0)
                            {
                                iterator1++;
                            }
                            break;
                        }
                    }
                    iterator1++;
                    if( (*iterator1).compare("#") == 0)
                    {
                        if (add_epsilon == 1)
                        {
                            if(!(std::find( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end(), "#") != ((*set_iterator1).current_sets).end()))
                            {
                                ((*set_iterator1).current_sets).push_back("#");
                            }
                        }
                    }
                }
            }
        }
        int flag = 0;
        for(set_iterator1 = first.begin(); set_iterator1!= first.end(); ++set_iterator1)
        {
            if ( ((*set_iterator1).current_sets).size() == ((*set_iterator1).previous_sets).size())
            {
                std::sort ( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end() );
                std::sort ( ((*set_iterator1).previous_sets).begin(), ((*set_iterator1).previous_sets).end() );
                if (!(std::equal( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end(), ((*set_iterator1).previous_sets).begin() )))
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
        for(set_iterator1 = first.begin(); set_iterator1!= first.end(); ++set_iterator1)
        {
            ((*set_iterator1).previous_sets).erase( ((*set_iterator1).previous_sets).begin(), ((*set_iterator1).previous_sets).end() );
            ((*set_iterator1).previous_sets).reserve(((*set_iterator1).current_sets).size());
            for(iterator3 = ((*set_iterator1).current_sets).begin(); iterator3 != ((*set_iterator1).current_sets).end(); ++iterator3)
            {
                ((*set_iterator1).previous_sets).push_back(*iterator3);
            }
        }
    }
    return 0;
}

int display_task2()
{
    int counter;
    std::vector<struct sets>::iterator iterator1;
    std::vector<std::string>::iterator iterator2;
    for(iterator1 = follow.begin(); iterator1 != follow.end(); ++iterator1)
    {
        counter = 1;
        std::sort ( ((*iterator1).current_sets).begin(), ((*iterator1).current_sets).end());
        cout<<"FOLLOW("<<(*iterator1).str<<") = { ";
        for(iterator2 = ((*iterator1).current_sets).begin(); iterator2 != ((*iterator1).current_sets).end(); ++iterator2)
        {
            cout<<(*iterator2);
            if(counter < ((*iterator1).current_sets).size())
               cout<<", ";
            counter++;
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
    std::vector<string>::iterator iterator1;
    std::vector<string>::iterator iterator2;
    std::vector<string>::iterator iterator3;
    std::vector<string>::iterator iterator4;
    std::vector<struct sets>::iterator set_iterator1;
    std::vector<struct sets>::iterator set_iterator2;
    std::vector<struct sets>::iterator set_iterator3;

    task1();
    for(iterator3 = non_terminal_vector.begin(); iterator3!= non_terminal_vector.end(); ++iterator3)
    {
        myset.str = (*iterator3);
        follow.push_back(myset);
    }
    //adding $ to the first non_terminal
    (follow[0].current_sets).push_back("$");
    while(start)
    {
        counter ++;
        for(set_iterator3 = follow.begin(); set_iterator3!= follow.end(); ++set_iterator3)
        {
            for(iterator1 = token_vector.begin(); iterator1 != token_vector.end(); ++iterator1)
            {
                if( (*iterator1).compare("#") != 0 && (*iterator1).compare("##") != 0)
                {
                    //set_iterator1 is B
                    for(set_iterator1 = follow.begin(); set_iterator1!= follow.end(); ++set_iterator1)
                    {
                        if( ((*set_iterator1).str).compare((*iterator1)) == 0 )
                        {
                            break;
                        }
                    }
                    iterator1++;
                    while( (*iterator1).compare("#") != 0 && (*iterator1).compare("##") != 0)
                    {
                        if( ((*set_iterator3).str).compare(*iterator1) == 0 )
                        {
                            iterator1++;
                            //checking for terminal
                            if(std::find( terminal_vector.begin(), terminal_vector.end(), *iterator1) != terminal_vector.end())
                            {
                                if(!(std::find( ((*set_iterator3).current_sets).begin(), ((*set_iterator3).current_sets).end(), *iterator1) != ((*set_iterator3).current_sets).end()))
                                {
                                    ((*set_iterator3).current_sets).push_back(*iterator1);
                                }
                            }
                            //adding first of C0 to follow of A
                            has_epsilon = 0;
                            add_epsilon = 0;
                            else if( std::find( non_terminal_vector.begin(), non_terminal_vector.end(), *iterator1) != non_terminal_vector.end() )
                            {
                                for( set_iterator2 = first.begin(); set_iterator2 != first.end(); ++set_iterator2)
                                {
                                    if( ((*set_iterator2).str).compare(*iterator1) == 0 )
                                    {
                                        for( iterator4 = ((*set_iterator2).current_sets).begin(); iterator4 != ((*set_iterator2).current_sets).end(); ++iterator4)
                                        {
                                            if ( (*iterator4).compare("#") != 0)
                                            {
                                                if(!(std::find( ((*set_iterator3).current_sets).begin(), ((*set_iterator3).current_sets).end(), *iterator4) != ((*set_iterator3).current_sets).end()))
                                                {
                                                    ((*set_iterator3).current_sets).push_back(*iterator4);
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
                                    iterator2 = iterator1;
                                    iterator2++;
                                    add_epsilon = 1;
                                    while( (*iterator2).compare("#") != 0 && (*iterator2).compare("##") != 0 && has_epsilon == 1)
                                    {
                                        if( std::find( non_terminal_vector.begin(), non_terminal_vector.end(), *iterator2) != non_terminal_vector.end() )
                                        {
                                            for( set_iterator2 = first.begin(); set_iterator2 != first.end(); ++set_iterator2)
                                            {
                                                if( ((*set_iterator2).str).compare(*iterator2) == 0 )
                                                {
                                                    has_epsilon = 0;
                                                    for( iterator4 = ((*set_iterator2).current_sets).begin(); iterator4 != ((*set_iterator2).current_sets).end(); ++iterator4)
                                                    {
                                                        if ( (*iterator4).compare("#") != 0)
                                                        {
                                                            if(!(std::find( ((*set_iterator3).current_sets).begin(), ((*set_iterator3).current_sets).end(), *iterator4) != ((*set_iterator3).current_sets).end()))
                                                            {
                                                                ((*set_iterator3).current_sets).push_back(*iterator4);
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
                                            iterator2++;
                                        }
                                        else
                                        {
                                            if(std::find( terminal_vector.begin(), terminal_vector.end(), *iterator2) != terminal_vector.end())
                                            {
                                                if(!(std::find( ((*set_iterator3).current_sets).begin(), ((*set_iterator3).current_sets).end(), *iterator2) != ((*set_iterator3).current_sets).end()))
                                                {
                                                    ((*set_iterator3).current_sets).push_back(*iterator2);
                                                }
                                            }
                                            add_epsilon = 0;
                                            break;
                                        }
                                    }
                                    if(add_epsilon == 1 && has_epsilon == 1)
                                    {
                                        for( iterator4 = ((*set_iterator1).current_sets).begin(); iterator4 != ((*set_iterator1).current_sets).end(); ++iterator4)
                                        {
                                            if(!(std::find( ((*set_iterator3).current_sets).begin(), ((*set_iterator3).current_sets).end(), *iterator4) != ((*set_iterator3).current_sets).end()))
                                            {
                                                ((*set_iterator3).current_sets).push_back(*iterator4);
                                            }
                                        }
                                    }
                                }
                            }
                            //adding follow of B to follow of A
                            else if( (*iterator1).compare("#") == 0)
                            {
                                for( iterator4 = ((*set_iterator1).current_sets).begin(); iterator4 != ((*set_iterator1).current_sets).end(); ++iterator4)
                                {
                                    if(!(std::find( ((*set_iterator3).current_sets).begin(), ((*set_iterator3).current_sets).end(), *iterator4) != ((*set_iterator3).current_sets).end()))
                                    {
                                        ((*set_iterator3).current_sets).push_back(*iterator4);
                                    }
                                }
                                break;
                            }
                            iterator1--;
                        }
                        iterator1++;
                    }
                }
            }
        }
        int flag = 0;
        for(set_iterator1 = follow.begin(); set_iterator1!= follow.end(); ++set_iterator1)
        {
            if ( ((*set_iterator1).current_sets).size() == ((*set_iterator1).previous_sets).size())
            {
                std::sort ( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end() );
                std::sort ( ((*set_iterator1).previous_sets).begin(), ((*set_iterator1).previous_sets).end() );
                if (!(std::equal( ((*set_iterator1).current_sets).begin(), ((*set_iterator1).current_sets).end(), ((*set_iterator1).previous_sets).begin() )))
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
        for(set_iterator1 = follow.begin(); set_iterator1!= follow.end(); ++set_iterator1)
        {
            ((*set_iterator1).previous_sets).erase( ((*set_iterator1).previous_sets).begin(), ((*set_iterator1).previous_sets).end() );
            ((*set_iterator1).previous_sets).reserve(((*set_iterator1).current_sets).size());
            for(iterator4 = ((*set_iterator1).current_sets).begin(); iterator4 != ((*set_iterator1).current_sets).end(); ++iterator4)
            {
                ((*set_iterator1).previous_sets).push_back(*iterator4);
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
            display_task0();
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
