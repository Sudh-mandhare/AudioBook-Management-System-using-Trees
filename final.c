#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 50
#define MAXINPUT 20

typedef struct AVL_Audio_book
{
    int Audio_book_id;
    char Title[MAXLEN];
    char Author[MAXLEN];
    char Narrator[MAXLEN];
    float duration;
    char Genere[MAXLEN];
    float sum_rating;
    int count_rating;

    struct AVL_Audio_book *left;
    struct AVL_Audio_book *right;
    int height;

} Audio_book_Tree;

typedef struct Audio_book_user_Tag
{
    int Audio_book_id;
    char Title[MAXLEN];
    char Author[MAXLEN];
    char Narrator[MAXLEN];
    float duration;
    char Genere[MAXLEN];
    float sum_rating;
    int count_rating;
    float timestamp;

    struct Audio_book_user_Tag *left;
    struct Audio_book_user_Tag *right;
    int height;

} Audio_book_user_Tree;

typedef struct User_Tree_Tag
{
    int User_id;
    char Name[MAXLEN];
    char Email[MAXLEN];
    char Narrator_preference[MAXLEN];
    char Author_preference[MAXLEN];
    char Genere_preference[MAXLEN];
    Audio_book_user_Tree *audiobooks;

    struct User_Tree_Tag *left;
    struct User_Tree_Tag *right;
    int height;

} User_Tree;

int parse(char s[])
{
    int num = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        num = num * 10 + (s[i] - 48);
    }
    return num;
}
int maxi(int l, int k)
{
    int ret_val;
    if(l > k)
    {
        ret_val = l;
    }
    else
    {
        ret_val = k;
    }
    return ret_val;
}

Audio_book_Tree *search(Audio_book_Tree *root, int id)
{
    Audio_book_Tree *temp = root;
    Audio_book_Tree *ans = NULL;
    int found = 0;
    while (temp != NULL && !found)
    {
        if (temp->Audio_book_id == id)
        {
            ans = temp;
            found = 1;
        }
        else if (temp->Audio_book_id < id)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }

    return ans;
}
Audio_book_user_Tree *search_abu(Audio_book_user_Tree *root, int id)
{
    Audio_book_user_Tree *temp = root;
    Audio_book_user_Tree *ans = NULL;
    int found = 0;
    while (temp != NULL && !found)
    {
        if (temp->Audio_book_id == id)
        {
            ans = temp;
            found = 1;
        }
        else if (temp->Audio_book_id < id)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }

    return ans;
}

Audio_book_Tree *LLrotation(Audio_book_Tree *root)
{
    Audio_book_Tree *temp = root->right;
    root->right = temp->left;
    temp->left = root;
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    int h = maxi(left, right);
    root->height = h + 1;
    int tleft = temp->left ? temp->left->height : -1;
    int tright = temp->right ? temp->right->height : -1;
    int th = maxi(tleft, tright);
    temp->height = th + 1;

    return temp;
}
Audio_book_Tree *RRrotation(Audio_book_Tree *root)
{

    Audio_book_Tree *temp = root->left;
    root->left = temp->right;
    temp->right = root;
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    int h = maxi(left, right);
    root->height = h + 1; // height is modified
    int tleft = temp->left ? temp->left->height : -1;
    int tright = temp->right ? temp->right->height : -1;
    int th = maxi(tleft, tright);
    temp->height = th + 1; // height is modified

    return temp;
}
Audio_book_Tree *insert_into_avl_abt(Audio_book_Tree *root, Audio_book_Tree *newnode)
{
    if (root == NULL)
    {
        root = newnode;
    }
    else
    {
        if (root->Audio_book_id > newnode->Audio_book_id)
        {

            root->left = insert_into_avl_abt(root->left, newnode);
        }
        else if (root->Audio_book_id < newnode->Audio_book_id)
        {
            root->right = insert_into_avl_abt(root->right, newnode);
        }
        int left = root->left ? root->left->height : 0;
        int right = root->right ? root->right->height : 0;
        root->height = maxi(left, right) + 1;
        if (abs(left - right) <= 1)
        {
            // it is avl no need of extra cases
            return root;
        }
        else
        {
            if (left > right)
            {
                int l = root->left->left ? root->left->left->height : -1;
                int r = root->left->right ? root->left->right->height : -1;

                if (r > l)
                {
                    root->left = LLrotation(root->left);
                    // lr
                }
                root = RRrotation(root);
                // ll
            }
            // right rotation
            else if (right > left)
            {
                int l = root->right->left ? root->right->left->height : -1;
                int r = root->right->right ? root->right->right->height : -1;
                if (l > r)
                {
                    root->right = RRrotation(root->right);
                    // rl
                }
                root = LLrotation(root);
                // rr
            }
        }
    }
    return root;
}
Audio_book_Tree *insert(Audio_book_Tree *root, Audio_book_Tree *newnode)
{
    Audio_book_Tree *temp = search(root, newnode->Audio_book_id);
    if (temp != NULL && root != NULL)
    {
        printf("duplicate node ");
        return root;
    }

    root = insert_into_avl_abt(root, newnode);

    return root;
}
Audio_book_Tree *create_audiobook_tree()
{
    Audio_book_Tree *head = NULL;

    FILE *fp = fopen("Audio_book_input.txt", "r");
    char word[MAXLEN];
    while (fscanf(fp, " %s", word) == 1)
    {
        Audio_book_Tree *temp = (Audio_book_Tree *)malloc(sizeof(Audio_book_Tree));
        temp->Audio_book_id = parse(word);
        fscanf(fp, "%s", word);
        strcpy(temp->Title, word);
        fscanf(fp, "%s", word);
        strcpy(temp->Author, word);
        fscanf(fp, "%s", word);
        strcpy(temp->Narrator, word);
        float q;
        fscanf(fp, "%f", &q);
        temp->duration = q;
        fscanf(fp, "%s", word);
        strcpy(temp->Genere, word);
        fscanf(fp, "%f", &q);
        temp->sum_rating = q;
        int num;
        fscanf(fp, "%d", &num);
        temp->count_rating = num;
        temp->left = NULL;
        temp->right = NULL;
        temp->height = 0;

        head = insert_into_avl_abt(head, temp);
    }
    fclose(fp);
    return head;
}

Audio_book_user_Tree *LLrotation_abu(Audio_book_user_Tree *root)
{
    Audio_book_user_Tree *temp = root->right;
    root->right = temp->left;
    temp->left = root;
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    int h = maxi(left, right);
    root->height = h + 1;
    int tleft = temp->left ? temp->left->height : -1;
    int tright = temp->right ? temp->right->height : -1;
    int th = maxi(tleft, tright);
    temp->height = th + 1;

    return temp;
}
Audio_book_user_Tree *RRrotation_abu(Audio_book_user_Tree *root)
{

    Audio_book_user_Tree *temp = root->left;
    root->left = temp->right;
    temp->right = root;
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    int h = maxi(left, right);
    root->height = h + 1;
    int tleft = temp->left ? temp->left->height : -1;
    int tright = temp->right ? temp->right->height : -1;
    int th = maxi(tleft, tright);
    temp->height = th + 1;

    return temp;
}
Audio_book_user_Tree *insert_into_avl_abu(Audio_book_user_Tree *root, Audio_book_user_Tree *newnode)
{
    if (root == NULL)
    {
        root = newnode;
    }
    else
    {
        if (root->Audio_book_id > newnode->Audio_book_id)
        {
            root->left = insert_into_avl_abu(root->left, newnode);
        }
        else if (root->Audio_book_id < newnode->Audio_book_id)
        {
            root->right = insert_into_avl_abu(root->right, newnode);
        }
        int left = root->left ? root->left->height : -1;
        int right = root->right ? root->right->height : -1;
        root->height = maxi(left, right) + 1;
        if (abs(left - right) <= 1)
        {
            // it is avl no need of extra cases
            return root;
        }
        else
        {
            if (left > right)
            {
                int l = root->left->left ? root->left->left->height : -1;
                int r = root->left->right ? root->left->right->height : -1;

                if (r > l)
                {
                    root->left = LLrotation_abu(root->left);
                }
                root = RRrotation_abu(root);
                
            }
            // right rotation
            else if (right > left)
            {
                int l = root->right->left ? root->right->left->height : -1;
                int r = root->right->right ? root->right->right->height : -1;
                if (l > r)
                {
                    root->right = RRrotation_abu(root->right);
                    
                }
                root = LLrotation_abu(root);
                
            }
        }
    }
    return root;
}
Audio_book_user_Tree *construct_user_audiolist(Audio_book_Tree *a, int id, Audio_book_user_Tree *abu)
{
    Audio_book_user_Tree *new_abu = (Audio_book_user_Tree *)malloc(sizeof(Audio_book_user_Tree));
    Audio_book_Tree *temp = search(a, id);

    new_abu->Audio_book_id = temp->Audio_book_id;
    strcpy(new_abu->Author, temp->Author);
    strcpy(new_abu->Narrator, temp->Narrator);
    strcpy(new_abu->Genere, temp->Genere);
    strcpy(new_abu->Title, temp->Title);
    new_abu->sum_rating = temp->sum_rating;
    new_abu->count_rating = temp->count_rating;
    new_abu->duration = temp->duration;
    new_abu->timestamp = 0.0;

    return insert_into_avl_abu(abu, new_abu);
}
User_Tree *LLrotation_user(User_Tree *root)
{
    User_Tree *temp = root->right;
    root->right = temp->left;
    temp->left = root;
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    int h = maxi(left, right);
    root->height = h + 1;
    int tleft = temp->left ? temp->left->height : -1;
    int tright = temp->right ? temp->right->height : -1;
    int th = maxi(tleft, tright);
    temp->height = th + 1;

    return temp;
}
User_Tree *RRrotation_user(User_Tree *root)
{

    User_Tree *temp = root->left;
    root->left = temp->right;
    temp->right = root;
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    int h = maxi(left, right);
    root->height = h + 1;
    int tleft = temp->left ? temp->left->height : -1;
    int tright = temp->right ? temp->right->height : -1;
    int th = maxi(tleft, tright);
    temp->height = th + 1;

    return temp;
}
User_Tree *insert_into_avl_user(User_Tree *root, User_Tree *newnode)
{
    if (root == NULL)
    {
        root = newnode;
    }
    else
    {
        if (root->User_id > newnode->User_id)
        {
            root->left = insert_into_avl_user(root->left, newnode);
        }
        else if (root->User_id < newnode->User_id)
        {
            root->right = insert_into_avl_user(root->right, newnode);
        }
        int left = root->left ? root->left->height : -1;
        int right = root->right ? root->right->height : -1;
        root->height = maxi(left, right) + 1;
        if (abs(left - right) <= 1)
        {
            // it is avl no need of extra cases
            return root;
        }
        else
        {
            if (left > right)
            {
                int l = root->left->left ? root->left->left->height : -1;
                int r = root->left->right ? root->left->right->height : -1;

                if (r > l)
                {
                    root->left = LLrotation_user(root->left);
                    // lr
                }
                root = RRrotation_user(root);
                // ll
            }
            // right rotation
            else if (right > left)
            {
                int l = root->right->left ? root->right->left->height : -1;
                int r = root->right->right ? root->right->right->height : -1;
                if (l > r)
                {
                    root->right = RRrotation_user(root->right);
                    // rl
                }
                root = LLrotation_user(root);
                // rr
            }
        }
    }
    return root;
}
User_Tree *create_user_tree(Audio_book_Tree *a)
{
    User_Tree *head = NULL;
    FILE *fp = fopen("users_input.txt", "r");
    char word[MAXLEN];
    while (fscanf(fp, " %s", word) == 1)
    {
        User_Tree *temp = (User_Tree *)malloc(sizeof(User_Tree));
        temp->User_id = parse(word);
        fscanf(fp, "%s", word);
        strcpy(temp->Name, word);
        fscanf(fp, "%s", word);
        strcpy(temp->Email, word);
        fscanf(fp, "%s", word);
        strcpy(temp->Narrator_preference, word);
        fscanf(fp, "%s", word);
        strcpy(temp->Author_preference, word);
        fscanf(fp, "%s", word);
        strcpy(temp->Genere_preference, word);

        Audio_book_user_Tree *abu = NULL;
        while (fscanf(fp, " %s", word) == 1 && strcmp(word, "end") != 0)
        {
            int k = parse(word);
            abu = construct_user_audiolist(a, k, abu);
        }
        temp->audiobooks = abu;

        temp->left = NULL;
        temp->right = NULL;
        temp->height = 0;
        head = insert_into_avl_user(head, temp);
    }
    fclose(fp);
    return head;
}

void inordertraversal_Audiobook(Audio_book_Tree *root)
{
    if (root != NULL)
    {
        inordertraversal_Audiobook(root->left);
        printf("ID:%d\t Title %s\n", root->Audio_book_id, root->Title);
        inordertraversal_Audiobook(root->right);
    }
    return;
}
void inordertraversal_Audiobook_users(Audio_book_user_Tree *root)
{
    if (root != NULL)
    {
        inordertraversal_Audiobook_users(root->left);
        printf("ID:%d\t Title %s\n", root->Audio_book_id, root->Title);
        inordertraversal_Audiobook_users(root->right);
    }
    return;
}
void inordertraversal_users(User_Tree *root)
{
    if (root != NULL)
    {
        inordertraversal_users(root->left);
        printf("ID:%d\t Name: %s\n", root->User_id, root->Name);
        inordertraversal_users(root->right);
    }
    return;
}
//function 13
void func13(Audio_book_Tree *root, int aid1, int aid2)
{
    if (root != NULL)
    {
        func13(root->left, aid1, aid2);
        if (root->Audio_book_id >= aid1 && root->Audio_book_id <= aid2)
        {
            // Print audiobook ID and title in table format
            printf("| %-12d | %-40s |\n", root->Audio_book_id, root->Title);
        }
        func13(root->right, aid1, aid2);
    }
}

void Range_Search_Audiobook(Audio_book_Tree *root)
{
    int aid1, aid2;
    printf("Enter the starting audiobook id: ");
    scanf("%d", &aid1);

    printf("Enter the ending audiobook id: ");
    scanf("%d", &aid2);

    if (aid2 < aid1)
    {
        // Swap values if aid2 is less than aid1
        int temp = aid1;
        aid1 = aid2;
        aid2 = temp;
    }

    // Table header
    printf("+--------------+------------------------------------------+\n");
    printf("| Audiobook ID | Title                                    |\n");
    printf("+--------------+------------------------------------------+\n");

    // Call func13 to print audiobooks within the specified range
    func13(root, aid1, aid2);

    // Table footer
    printf("+--------------+------------------------------------------+\n");
}
// function 12
int func11AudioUserbook(Audio_book_user_Tree *root, int aid, int total_time)
{
    if (root != NULL)
    {
        total_time = func11AudioUserbook(root->left, aid, total_time);

        if (root -> Audio_book_id == aid)
        {
            printf("AudioBok details : \n");
            printf("AudioBook ID : %d", root->Audio_book_id);
            printf("Audiboom title : %s", root->Title);
            printf("Audiobook Author : %s", root->Author);

            total_time = total_time + root->timestamp;
        }
        

        total_time = func11AudioUserbook(root->right, aid, total_time);
    }

    return total_time;
}
int func11User(User_Tree *root, int aid, int total_time)
{
    if (root != NULL)
    {
        total_time = func11User(root->left, aid, total_time);

        total_time = func11AudioUserbook(root->audiobooks, aid, total_time);

        total_time = func11User(root->right, aid, total_time);
    }
    return total_time;
}

void Listening_history_report_audiobook(User_Tree *user_root)
{
    int aid;
    printf("Enter the audiobook whose report you need : ");
    scanf("%d",&aid);
    int total_time = func11User(user_root, aid, 0);

    printf("Total time : %d", total_time);
}

//function 10
int funct10_inordertraverse_Audibook_user(Audio_book_user_Tree *root, int total_time)
{
    if (root != NULL)
    {
        total_time = funct10_inordertraverse_Audibook_user(root->left, total_time);

        // Print information of the current Audio_book_user node in table format
        printf("| %-40s | %-20s | %-10.2f |\n",
               root->Title, root->Author, root->timestamp);

        total_time += root->timestamp;

        total_time = funct10_inordertraverse_Audibook_user(root->right, total_time);
    }
    return total_time;
}
User_Tree *search_user(User_Tree *root, int id)
{
    User_Tree *temp = root;
    User_Tree *ans = NULL;
    int found = 0;
    while (temp != NULL && !found)
    {
        if (temp->User_id == id)
        {
            ans = temp;
            found = 1;
        }
        else if (temp->User_id < id)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }

    return ans;
}
void Listening_history_report_user(User_Tree *user_root)
{
    int uid;
    printf("ENter the user id : ");
    scanf("%d",&uid);

    User_Tree* uptr = search_user(user_root,uid);
    printf("+------------------------------------------+---------------------+------------+\n");
    printf("| %-40s | %-20s | %-10s |\n", "Title", "Author", "Timestamp");
    printf("+------------------------------------------+---------------------+------------+\n");

    int total_time = funct10_inordertraverse_Audibook_user(uptr->audiobooks, 0);

    printf("+------------------------------------------+---------------------+------------+\n");
    printf("| Total Listening Time: %-56d |\n", total_time);
    printf("+------------------------------------------+---------------------+------------+\n");
}


// function  9

Audio_book_Tree *max_rate_ptr(Audio_book_Tree *root, int *max_rate)
{
    if (root == NULL)
        return NULL;

    Audio_book_Tree *left_max = max_rate_ptr(root->left, max_rate);
    Audio_book_Tree *right_max = max_rate_ptr(root->right, max_rate);

    int rate = 0;
    if (root->count_rating > 0)
    {
        rate = root->sum_rating / root->count_rating;
    }

    Audio_book_Tree *ret_ptr = root;

    if (rate > *max_rate)
    {
        *max_rate = rate;
        ret_ptr = root;
    }

    if (left_max != NULL && (left_max->sum_rating / left_max->count_rating) > *max_rate)
    {
        *max_rate = left_max->sum_rating / left_max->count_rating;
        ret_ptr = left_max;
    }

    if (right_max != NULL && (right_max->sum_rating / right_max->count_rating) > *max_rate)
    {
        *max_rate = right_max->sum_rating / right_max->count_rating;
        ret_ptr = right_max;
    }

    return ret_ptr;
}
void Most_Popular_Audiobook(Audio_book_Tree *root)
{
    int max_rate = 0;
    Audio_book_Tree *most_popular = max_rate_ptr(root, &max_rate);

    if (most_popular != NULL)
    {
        printf("Most Popular Audiobook:\n");
        printf("Title: %s\n", most_popular->Title);
        printf("Author: %s\n", most_popular->Author);
        printf("Average Rating: %.2f\n", (float)most_popular->sum_rating / most_popular->count_rating);
    }
    else
    {
        printf("No audiobooks found.\n");
    }
}

// function 8

Audio_book_user_Tree *search_audiobook_user(Audio_book_user_Tree *root, int aid)
{
    Audio_book_user_Tree *temp = root;
    Audio_book_user_Tree *ans = NULL;
    int found = 0;
    while (temp != NULL && !found)
    {
        if (temp->Audio_book_id == aid)
        {
            ans = temp;
            found = 1;
        }
        else if (temp->Audio_book_id < aid)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }

    return ans;
}
void update_audiobook_from_users(User_Tree *users, float sum_rating, int count_rating, int id)
{
    if (users != NULL)
    {
        update_audiobook_from_users(users->left, sum_rating, count_rating, id);
        Audio_book_user_Tree *abu = search_abu(users->audiobooks, id); // getting audiobook from user audiolist
        if (abu != NULL)
        {
            // updating ratings
            abu->sum_rating = sum_rating;
            abu->count_rating = count_rating;
        }
        update_audiobook_from_users(users->right, sum_rating, count_rating, id);
    }
}
void Rate_Audiobook(User_Tree *user_root, Audio_book_Tree *abt_root)
{
    int uid, aid, rating;
    printf("Enter the user who will rate the audiobook : ");
    scanf("%d", &uid);

    printf("Enter the audio id : ");
    scanf("%d", &aid);

    printf("Enter your rating ut of 5 : ");
    scanf("%d", &rating);

    User_Tree *uptr = search_user(user_root, uid);
    Audio_book_user_Tree *abu_ptr = uptr->audiobooks;

    abu_ptr = search_audiobook_user(abu_ptr, aid);
    abu_ptr->sum_rating += rating;
    abu_ptr->count_rating != 1;
    // updated in the present one

    int new_sum_rating = abu_ptr->sum_rating;
    int new_count_rting = abu_ptr->count_rating;

    // now update everywhere
    // 1. in audioboook tree
    update_audiobook_from_users(user_root,new_sum_rating,new_count_rting,aid);

}

// function 7 - > sort according to timestap is left

void inordertraverse_Audibook_user(Audio_book_user_Tree *root)
{
    if (root != NULL)
    {
        inordertraverse_Audibook_user(root->left);

        // Print information of the current Audio_book_user node in table format
        printf("| %-12d | %-40s | %-20s | %-20s | %-8.2f | %-15s | %-10.2f | %-15d | %-10.2f |\n",
               root->Audio_book_id, root->Title, root->Author, root->Narrator,
               root->duration, root->Genere, root->sum_rating, root->count_rating, root->timestamp);

        inordertraverse_Audibook_user(root->right);
    }
    return;
}
void inordertraverse_Audibook_user_genreFilter(Audio_book_user_Tree *root, char string[])
{
    if (root != NULL)
    {
        inordertraverse_Audibook_user(root->left);

        // Print information of the current Audio_book_user node in table format
        if (strcmp(root->Genere, string) == 0)
        {
            printf("| %-12d | %-40s | %-20s | %-20s | %-8.2f | %-15s | %-10.2f | %-15d | %-10.2f |\n",
                   root->Audio_book_id, root->Title, root->Author, root->Narrator,
                   root->duration, root->Genere, root->sum_rating, root->count_rating, root->timestamp);
        }

        inordertraverse_Audibook_user(root->right);
    }
    return;
}

void Display_user_libraries(User_Tree *user_root)
{
    int uid;
    printf("Enter the user id : ");
    scanf("%d", &uid);

    int choice;
    printf("Enter the way of filter : \n 1. ALL \n 2. BY GENRE \n Enter yout chouce :  ");
    scanf("%d", &choice);

    User_Tree *uptr = search_user(user_root, uid);
    
    Audio_book_user_Tree *abu_root = uptr->audiobooks; // original tree - make a copy of it based on timestamp

    printf("| %-12s | %-20s | %-20s | %-20s | %-8s | %-15s | %-7s | %-7s | %-5s |\n",
           "Audio ID", "Title", "Author", "Narrator", "Duration", "Genre", "Sum Rating", "Rating Count", "Timestamp");
    printf("|--------------|------------------------------------------|---------------------|---------------------|----------|-------------------|-------------|-------------------|------------|\n");
    
    
    if (choice == 1)
    {
        inordertraverse_Audibook_user(abu_root);
    }
    else if (choice == 2)
    {
        char string[MAXLEN];
        printf("Enter the genre : ");
        scanf("%s", string);

        inordertraverse_Audibook_user_genreFilter(abu_root, string);
    }
}

// function 6
void Listening_progess(Audio_book_Tree *audio_tree_root, User_Tree *user_root)
{
    int uid, aid, new_timestamp;
    printf("Enter the user id : ");
    scanf("%d", &uid);

    // find here and update only - as listeming progress only for a specific user

    User_Tree *uptr = search_user(user_root, uid);

    // display this users audiobooks
    printf("THESE ARE THE AUDIOBOOKS FOR THIS USER : \n \n ");
    inordertraversal_Audiobook_users(uptr->audiobooks);

    printf("\n \n ");

    printf("Enter the audio id : ");
    scanf("%d", &aid);

    printf("Enter the timestamp : ");
    scanf("%d", &new_timestamp);

    Audio_book_user_Tree *abu_root = uptr->audiobooks;
    Audio_book_user_Tree *abu_ptr = search_audiobook_user(abu_root, aid);

    abu_ptr->timestamp = new_timestamp; // updated timestamp
}

//function 5
void inordertraver_Audiobook_choice(Audio_book_Tree *root, int choice, char string[])
{
    if (root != NULL)
    {
        if ((choice == 1 && strcmp(root->Title, string) == 0) || (choice == 2 && strcmp(root->Author, string) == 0) || (choice == 3 && strcmp(root->Genere, string) == 0))
        {
            printf("ID:%d\t Title %s\n", root->Audio_book_id, root->Title);
        }
        // without else will also search for duplicates
        inordertraver_Audiobook_choice(root->left, choice, string);

        inordertraver_Audiobook_choice(root->right, choice, string);
    }
    return;
}

void Search_audiobook(Audio_book_Tree *root)
{
    int choice;
    printf("YOU WANT TO SEARCH ON :\n1.Title\n2.Autho\n3.Genre\n ENTER YOUR CHOICE : ");
    scanf("%d", &choice);

    char string[MAXLEN];
    printf("Enter the string you want to search : ");
    scanf("%s", string);

    inordertraver_Audiobook_choice(root, choice, string);
}

//function 2 -- 
Audio_book_Tree *delete_abt_helper(Audio_book_Tree *root, Audio_book_Tree *smallparent)
{
    if (root->Audio_book_id > smallparent->Audio_book_id)
    {

        root->left = delete_abt_helper(root->left, smallparent);
    }
    else if (root->Audio_book_id < smallparent->Audio_book_id)
    {

        root->right = delete_abt_helper(root->right, smallparent);
    }
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    root->height = maxi(left, right) + 1;
    if (abs(left - right) <= 1)
    {
        // it is avl no need of extra cases
        return root;
    }
    else if (root != NULL)
    {
        if (left > right)
        {
            int l = root->left->left ? root->left->left->height : -1;
            int r = root->left->right ? root->left->right->height : -1;

            if (r > l)
            {
                root->left = LLrotation(root->left);
                // lr
            }
            root = RRrotation(root);
            // ll
        }
        // right rotation
        else if (right > left)
        {
            int l = root->right->left ? root->right->left->height : -1;
            int r = root->right->right ? root->right->right->height : -1;
            if (l > r)
            {
                root->right = RRrotation(root->right);
                // rl
            }
            root = LLrotation(root);
            // rr
        }
    }
    return root;
}
void delete_Audio_book_bst(Audio_book_Tree **root)
{
    Audio_book_Tree *nptr = (*root);
    if (nptr != NULL)
    {
        if (nptr->right == NULL && nptr->left == NULL)
        {
            (*root) = NULL;
            free(nptr);
        }
        else if (nptr->right == NULL)
        {
            (*root) = nptr->left;

            free(nptr);
        }
        else if (nptr->left == NULL)
        {
            (*root) = nptr->right;

            free(nptr);
        }
        else
        {

            Audio_book_Tree *prev, *ptr;
            // take left go to right of left
            prev = nptr->left;
            ptr = nptr->left;
            while (ptr->right != NULL)
            {
                prev = ptr;
                ptr = ptr->right;
            }
            if (ptr == prev)
            {
                ptr->right = nptr->right;
                free(nptr);

                // i have to balance tree fr ptr only
            }
            else
            {
                int l = prev->left ? prev->left->height : -1;
                int r = ptr->left ? ptr->left->height : -1;

                prev->right = ptr->left;
                prev->height = 1 + maxi(l, r); // changed ptr to prev

                ptr->right = nptr->right;
                ptr->left = nptr->left;

                free(nptr);
                // set function
            }

            (*root) = delete_abt_helper(ptr, prev);
        }
    }

    return;
}
Audio_book_Tree *delete_abt_rec(Audio_book_Tree *root, int id)
{
    if (root->Audio_book_id == id)
    {
        // Audiobook with given ID found
        delete_Audio_book_bst(&root);
        return root;
    }
    else if (root->Audio_book_id > id)
    {
     
        root->left = delete_abt_rec(root->left, id);
    }
    else
    {
        
        root->right = delete_abt_rec(root->right, id);
    }
    int left = root->left ? root->left->height : -1;
    int right = root->right ? root->right->height : -1;
    root->height = maxi(left, right) + 1; 
    if (abs(left - right) <= 1)
    {
        
        return root;
    }
    else if (root != NULL)
    {
        if (left > right)
        {
        
            int l = root->left->left ? root->left->left->height : -1;
            int r = root->left->right ? root->left->right->height : -1;

            if (r > l)
            {
                // child is right lopsided
                // adjusting the child to make it left lopsided
                root->left = LLrotation(root->left);
                // lr
            }
            root = RRrotation(root);
            // adjusting the left unbalanced tree
        }
        else if (right > left)
        {
            // right rotation
            int l = root->right->left ? root->right->left->height : -1;
            int r = root->right->right ? root->right->right->height : -1;
            if (l > r)
            {
                // child is left lopsided
                // adjusting the child to make it right lopsided
                root->right = RRrotation(root->right);
                // rl
            }
            root = LLrotation(root);
            // adjusting the right unbalanced tree
        }
    }
    return root;
}
Audio_book_Tree *delete_abt(Audio_book_Tree *root, int id)
{
    // checking whether audiobook is present or not
    Audio_book_Tree *temp = search(root, id);
    if (temp != NULL)
    {
        // calling delete audiobook recursively
        root = delete_abt_rec(root, id);
    }
    else
    {
        printf("ID doesn't exist\n");
    }

    return root;
}
void Delete_Audio_book_helper(Audio_book_Tree *root, int n, char word[], int array[], int *size)
{
    // inorder traversal to get IDs
    if (root != NULL)
    {
        Delete_Audio_book_helper(root->left, n, word, array, size);
        if ((n == 1 && strcmp(root->Title, word) == 0) || (n == 2 && strcmp(root->Author, word) == 0))
        {
            // If given criteria matches,storing ID in array
            array[*size] = root->Audio_book_id;
            (*size) = (*size) + 1;
        }
        Delete_Audio_book_helper(root->right, n, word, array, size);
    }
    return;
}
void Delete_Audio_book(Audio_book_Tree **mainroot)
{
    Audio_book_Tree *root = (*mainroot);
    char word[MAXLEN];
    int n;
    // providing user multiple options for deletion
    printf("Choose one of the option\n");
    printf("1.Delete by title\n");
    printf("2.Delete by author\n");
    scanf("%d", &n);
    if (n == 1)
    {
        printf("Enter the title\n");
        scanf("%s", word);
    }
    else if (n == 2)
    {
        printf("Enter the author\n");
        scanf("%s", word);
    }
    else
    {
        printf("Invalid option\n");
        return;
    }
    int array[MAXINPUT] = {0}; // Creating an array to store IDs of audiobooks to be deleted
    int size = 0;
    // Calling helper function to store IDs of audiobooks to be deleted
    Delete_Audio_book_helper(root, n, word, array, &size);
    int i = 0;
    while (i < size)
    {
        // calling delete function to delete all audiobooks with IDs in array
        root = delete_abt(root, array[i]);
        i++;
    }

    if (n == 1 && i == 0)
    {
        printf("no audio book found with the title %s ", word);
    }
    else if (n == 2 && i == 0)
    {
        printf("no audio book found with the author  %s ", word);
    }
    else
    {

        printf("deletion done successfully");
    }
    (*mainroot) = root;
    return;
}
//function 2 --end


//function 4
void AddToUserlibrary(Audio_book_Tree *root, User_Tree *user_root)
{
    int uid;
    printf("Enter the user to whm you wnat to enter a audiobook  : ");
    scanf("%d", &uid);

    User_Tree *u_ptr = search_user(user_root, uid);
    Audio_book_user_Tree *abu_root = u_ptr->audiobooks;

    // search audiobook based on audiobook it - make copy of it nd inset in avl tree of it
    int aid;
    printf("Enter the audiobook id : ");
    scanf("%d", &aid);

    Audio_book_Tree *abt_ptr = search(root, aid);

    Audio_book_user_Tree *user_book = (Audio_book_user_Tree *)malloc(sizeof(Audio_book_user_Tree));
    if (user_book == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    // Copy elements of audiobook_ptr into user_book
    user_book->Audio_book_id = abt_ptr->Audio_book_id;
    strcpy(user_book->Title, abt_ptr->Title);
    strcpy(user_book->Author, abt_ptr->Author);
    strcpy(user_book->Narrator, abt_ptr->Narrator);
    user_book->duration = abt_ptr->duration;
    strcpy(user_book->Genere, abt_ptr->Genere);
    user_book->sum_rating = abt_ptr->sum_rating;
    user_book->count_rating = abt_ptr->count_rating;
    user_book->timestamp = 0.0; // Assuming timestamp is initialized to zero

    user_book->left = NULL;
    user_book->right = NULL;
    user_book->height = 1; // Assuming the height of a new node is 1

    // Insert the new audiobook entry into the AVL tree of the user's library
    // You will need to implement the Insert_User function to insert into the user's AVL tree

    printf("DONE \n");

    abu_root = insert_into_avl_abu(abu_root, user_book); // assuming this fucntio returns the root to it
    printf("DONE \n");

    u_ptr->audiobooks = abu_root;
}

//function 1
void Add_audiobook(Audio_book_Tree *audio_root)
{
    Audio_book_Tree *new_ptr = (Audio_book_Tree *)malloc(sizeof(Audio_book_Tree));

    if (new_ptr == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    new_ptr->left = NULL;
    new_ptr->right = NULL;
    new_ptr->height = 1;

    printf("Enter Audio book ID: ");
    scanf("%d", &(new_ptr->Audio_book_id));
    printf("Enter Title: ");
    scanf("%s", new_ptr->Title);
    printf("Enter Author: ");
    scanf("%s", new_ptr->Author);
    printf("Enter Narrator: ");
    scanf("%s", new_ptr->Narrator);
    printf("Enter Duration: ");
    scanf("%f", &(new_ptr->duration));
    printf("Enter Genre: ");
    scanf("%s", new_ptr->Genere);
    new_ptr->sum_rating = 0.0; // Initialize sum_rating to 0
    new_ptr->count_rating = 0; // Initialize count_rating to 0

    // Insert the new audiobook into the AVL tree
    audio_root = insert(audio_root, new_ptr);
}

void Edit_audiobook(Audio_book_Tree *abt)
{
    // searching for audiobook with given ID in the audiolist
    int id;
    printf("Enter ID of audibook you want to edit : ");
    scanf("%d", &id);
    Audio_book_Tree *temp = search(abt, id);
    if (temp == NULL)
    {
        printf("Audiobook with ID %d not found\n", id);
    }
    else
    {
        // Editing the audiobook with given ID
        int t = 1, n;
        while (t != 0)
        {
            printf("Which field you want to edit for audiobook with ID %d\n", id);
            printf("0.Exit\n");
            printf("1.Title\n");
            printf("2.Auhtor\n");
            printf("3.Duration\n");
            scanf("%d", &n);
            if (n == 1)
            {
                printf("Enter new title\n");
                scanf("%s", temp->Title);
                printf("Title edited successfully\n");
            }
            else if (n == 2)
            {
                printf("Enter new Auhtor\n");
                scanf("%s", temp->Author);
                printf("Author edited successfully\n");
            }
            else if (n == 3)
            {
                printf("Enter new duration\n");
                scanf("%f", &temp->duration);
                printf("Duration edited successfully\n");
            }
            else if (n == 0)
            {
                t = 0;
            }
        }
    }
}
//-- function 1

//function 3
User_Tree *search_user_id(User_Tree *root, int id)
{
    User_Tree *temp = root;
    User_Tree *ans = NULL;
    int found = 0;
    while (temp != NULL && !found)
    {
        if (temp->User_id == id)
        {
            ans = temp;
            found = 1;
        }
        else if (temp->User_id < id)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    return ans;
}
User_Tree *search_user_email(User_Tree *root, char email[])
{
    User_Tree *ans = NULL;
    if (root != NULL)
    {
        if (strcmp(email, root->Email) == 0)
            ans = root;
        if (ans == NULL)
        {
            User_Tree *left_ans = search_user_email(root->left, email);
            User_Tree *right_ans = search_user_email(root->right, email);
            if (left_ans)
                ans = left_ans;
            else
                ans = right_ans;
        }
    }
    return ans;
}
void add_user_profile(User_Tree **uth)
{
    int id;
    char email[MAXLEN];
    char word[MAXLEN];
    User_Tree *ut = (*uth);
    printf("Enter ID for new user:");
    scanf("%d", &id);
    printf("Enter email for new user:");
    scanf("%s", email);
    // searching if user with given ID exists
    if (search_user_id(ut, id) != NULL)
        printf("User ID already exist\n");
    // searching if user with given email exists
    else if (search_user_email(ut, email) != NULL)
        printf("Email already exist\n");
    else
    {
        // Creating new user
        User_Tree *new_user = (User_Tree *)malloc(sizeof(User_Tree));
        new_user->User_id = id;
        strcpy(new_user->Email, email);
        printf("Enter Name for user:");
        scanf("%s", new_user->Name);
        printf("Enter author preference:");
        scanf("%s", new_user->Author_preference);
        printf("Enter genre preference:");
        scanf("%s", new_user->Genere_preference);
        printf("Enter narrator preference:");
        scanf("%s", new_user->Narrator_preference);
        new_user->right = NULL;
        new_user->left = NULL;
        new_user->height = 0;
        // Inserting user in user avl tree
        ut = insert_into_avl_user(ut, new_user);
        printf("User added successfully\n");
        (*uth) = ut;
    }
}
//--function 3


int main()
{
    Audio_book_Tree *Audio_list = create_audiobook_tree();
    User_Tree *Users_list = create_user_tree(Audio_list);
    
    //Add_audiobook(Audio_list);                                  //done
    //Edit_audiobook(Audio_list);                                     //done
    // Delete_Audio_book(&Audio_list);                                //done
    //add_user_profile(&Users_list);                              // done
    //AddToUserlibrary(Audio_list,Users_list);                       //done 2 -10
    inordertraversal_Audiobook(Audio_list);
    inordertraversal_users(Users_list);
    // Search_audiobook(Audio_list);                                  //done
    //Listening_progess(Audio_list, Users_list);                         //done
    //Display_user_libraries(Users_list);                                 //done
    //Rate_Audiobook(Users_list, Audio_list);                         //done - can  update in audiobook tree
   // Most_Popular_Audiobook(Audio_list);                                 //done
    //Listening_history_report_user(Users_list);                         //done
    //Listening_history_report_audiobook(Users_list);                        //done - formatting
    Range_Search_Audiobook(Audio_list);                                         //done

   
    return 0;
}