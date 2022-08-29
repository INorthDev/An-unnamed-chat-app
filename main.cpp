#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <exception>

using namespace std;

class LowLevelAuth: public exception
{
public:
        [[nodiscard]] const char* what() const noexcept override
        {
            return "You don't have enough Authorization level!";
        }
};

class User
{
    protected:
        string display_name;
        int cur_id;
    public:
        static int id;

        friend bool operator != (const User& u1, const User& u2)
        {
            if(u1.getCurId() != u2.getCurId()){
                return true;
            }
            return false;
        }

        [[nodiscard]] int getCurId() const
        {
            return cur_id;
        }

        string getDisplayName()
        {
            return display_name;
        }

        void changeDisplayName(string new_name, const User& auth)
        {
            if(getCurId() != auth.getCurId())
            {
                throw LowLevelAuth();
            }
            display_name = std::move(new_name);
        }

    explicit User(string name): display_name(std::move(name))
  {
        cur_id = id;
        id++;
  }
};

int User::id = 1;

class Comment
{
    protected:
        User author;
        string message;
        int cur_id;
        vector<Comment> replies;

        friend bool operator == (const Comment& com1, const Comment& com2)
        {
            if(com1.cur_id == com2.cur_id)
            {
                return true;
            }
            return false;
        }
    public:
        static int id;

        vector<Comment> getReplies()
        {
            return replies;
        }

        string getMessage()
        {
            return message;
        }

        User getAuthor()
        {
            return author;
        }

        void sendReply(const Comment& reply)
        {
            replies.push_back(reply);
        }

        void editComment(string new_message)
        {
            message = std::move(new_message);
        }

    Comment(User au, string m, vector<Comment> reps): author(std::move(au)), message(std::move(m))
    {
        cur_id = id;
        replies = std::move(reps);

        id++;
    }
};

int Comment::id = 1;

class Content
{
    private:
        string display_name;
        int cur_id;
        vector<Comment> logs;
    public:
        static int id;

        string getDisplayName()
        {
            return display_name;
        }

        [[nodiscard]] int getCurId() const
        {
            return cur_id;
        }

        void sendReply(const Comment& comment, const Comment& reply)
        {
            logs.at(findCommentIndex(comment)).sendReply(reply);
        }

        void sendComment(const Comment& comment)
        {
            logs.push_back(comment);
        }

        void deleteComment(Comment& comment)
        {
            logs.erase(logs.begin() + findCommentIndex(comment));
        }

        int findCommentIndex(const Comment& com)
        {
            for(int i=0; i<logs.size(); i++)
            {
                if(logs.at(i) == com)
                {
                    return i;
                }
            }
            return -1;
        }

        void editComment(const Comment& comment, const string& new_message, const User& auth)
        {
            int idx = findCommentIndex(comment);
            if(logs.at(idx).getAuthor() != auth)
            {
                throw LowLevelAuth();
            }
            logs.at(idx).editComment(new_message);
        }

        void displayContent(const bool del = false)
        {
            if(del)
            {
                system("CLS");
            }
            cout << getDisplayName() << '[' << getCurId() << ']' << endl;
            for(auto cur : logs)
            {
                cout << '\t' << cur.getAuthor().getDisplayName() << '[' << cur.getAuthor().getCurId() << ']' << " - " << cur.getMessage() << endl;
                for(auto rep : cur.getReplies())
                {
                    cout << "\t\t" << rep.getAuthor().getDisplayName() << '[' << rep.getAuthor().getCurId() << ']' << " - " << rep.getMessage() << endl;
                }
            }
        }

    Content(vector<Comment> lgs, string dn): logs(std::move(lgs)), display_name(std::move(dn))
    {
        cur_id = id;
        id++;
    }
};

int Content::id = 1;

int main()
{

    vector<Comment> mesajlar;

    User ali = User("Ali");
    User mehmet = User("Mehmet");
    Content sinif_grubu = Content(mesajlar, "50-F FBI SUBESI");

    vector<Comment> cvplr;

    Comment alinin_mesaji = Comment(ali, "Sa", cvplr);

    sinif_grubu.sendComment(alinin_mesaji);
    sinif_grubu.sendReply(alinin_mesaji, Comment(mehmet, "As", cvplr));
    sinif_grubu.displayContent(true);
    sinif_grubu.editComment(alinin_mesaji, "As yazan sus", ali);
    sinif_grubu.displayContent(true);
    return 0;
}
