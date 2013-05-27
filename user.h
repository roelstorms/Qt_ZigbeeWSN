#ifndef USER_H
#define USER_H

class user
{
  private:
  int id;
  static int next_id;

  public:
  static int next_user_id()
  {
    next_id++;
    return next_id;
  }
  /* More stuff for the class user */
  user()
  {
    id = user::next_id++; //or, id = user.next_user_id();
  }
};
int user::next_id = 0;

#endif // USER_H
