#include "query_funcs.h"
#include <iomanip>
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
  work W(*C);
  //W.exec(query);

  string find_max ("SELECT PLAYER_ID FROM PLAYER WHERE player_id = (SELECT max(player_id) FROM player)");
  string query;

  result f_max = W.exec(find_max);
  int max = 0;
  for(auto r:f_max){
    max = r[0].as<int>();
  }
  max++;
  
 
  query += "INSERT INTO player (player_id,team_id,uniform_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg)";
  query += " VALUES(" + to_string(max) + ", " + to_string(team_id) + ", " + to_string(jersey_num) + ", '" + first_name + "', '" + last_name + "', " + to_string(mpg) + ", " + to_string(ppg) + ", " + to_string(rpg) + ", " + to_string(apg) + ", " + to_string(spg) + ", " + to_string(bpg) +");";
  W.exec(query);
  W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
  work W(*C);
  string find_max ("SELECT team_id FROM team WHERE team_id = (SELECT max(team_id) FROM team)");
  string query;

  result f_max = W.exec(find_max);
  int max = 0;
  for(auto r:f_max){
    max = r[0].as<int>();
  }
  max++;
  query += "INSERT INTO team (team_id, name, state_id, color_id, wins, losses)";
  query += " VALUES(" + to_string(max) + " , '" + name + "', " + to_string(state_id) + ", " + to_string(color_id) + ", " + to_string(wins) + ", " + to_string(losses) + ");";
  W.exec(query);
  W.commit();

}


void add_state(connection *C, string name)
{
  work W(*C);
 
  string find_max ("SELECT state_id FROM state WHERE state_id = (SELECT max(state_id) FROM state)");
  string query;

  result f_max = W.exec(find_max);
  int max = 0;
  for(auto r:f_max){
    max = r[0].as<int>();
  }
  max++;

  query += "INSERT INTO state (state_id, name) ";

  query += " VALUES(" + to_string(max) + " , '" + name + "'); ";
  W.exec(query);
  W.commit();

}


void add_color(connection *C, string name)
{
  work W(*C);
  //W.exec(query);                                                                                                           \
                                                                                                                              

  string find_max ("SELECT color_id FROM color WHERE color_id = (SELECT max(color_id) FROM color)");
  string query;

  result f_max = W.exec(find_max);
  int max = 0;
  for(auto r:f_max){
    max = r[0].as<int>();
  }
  max++;

  query += "INSERT INTO color (color_id, name)";
  query += " VALUES(" + to_string(max) + " , '" + name + "');";
  W.exec(query);
  W.commit();

}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  string query;
  query += "SELECT * FROM player WHERE ";
  if(use_mpg){
    query  += string( "(mpg BETWEEN ") + to_string(min_mpg) + " AND " + to_string(max_mpg) + ')';
  }
  if(use_ppg){
    query  += string( "(ppg BETWEEN ") + to_string(min_ppg) + " AND " + to_string(max_ppg) + ')';
  }
  if(use_rpg){
    query  += string( "(rpg BETWEEN ") + to_string(min_rpg) + " AND "	+ to_string(max_rpg) + ')';
  }
  if(use_apg){
    query  += string( "(apg BETWEEN ") + to_string(min_apg) + " AND "	+ to_string(max_apg) + ')';
  }
  if(use_spg){
    query  += string( "(spg BETWEEN ") + to_string(min_spg) + " AND "	+ to_string(max_spg) + ')';
  }
  if(use_bpg){
    query  += string( "(bpg BETWEEN ") + to_string(min_bpg) + " AND "	+ to_string(max_bpg) + ')';
  }
  if((use_mpg||use_ppg||use_rpg||use_apg||use_spg||use_bpg) == 0){
    query = "SELECT * FROM player ";
  }
  query += ';';

  //execute sql query
  work W(*C);

  result R = W.exec(query);
  cout << setprecision(1) << fixed;
  //print out the output
  cout <<"PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl; 
   for(auto c:R)
      cout << c[0].as<int>() <<' '
      << c[1].as<int>() << ' '
      << c[2].as<int>() << ' '
      << c[3].as<string>() << ' '
      << c[4].as<string>() << ' '
      << c[5].as<int>() << ' '
      << c[6].as<int>() << ' '
      << c[7].as<int>() << ' '
      << c[8].as<int>() << ' '
      << c[9].as<double>() << ' '
      << c[10].as<double>() << ' ' << endl;     
  
 
}
void query2(connection *C, string team_color)
{
  string query;
  query += "SELECT team.name FROM team,color WHERE (team.color_id = color.color_id) AND color.name=";
  query += " '" + team_color + "'";
  query += " ;";
  work W(*C);
  result R = W.exec(query);
  cout << "NAME" << endl;
  for(auto c:R)
    cout << c[0].as<string>() <<endl;
}


void query3(connection *C, string team_name)
{
  string query;
  query += "SELECT first_name,last_name FROM player,team WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND team.name = ";
  query += " '" + team_name + "' ";
  query += "ORDER BY ppg " ;
  query += "DESC;";
  work W(*C);
  result R = W.exec(query);
  cout << "FIRST_NAME LAST_NAME" <<endl;
  for(auto c:R)
    cout << c[0].as<string>() << ' '
	 << c[1].as<string>() << ' '<< endl;
}


void query4(connection *C, string team_state, string team_color)
{
  string query;
  query += "SELECT first_name,last_name,uniform_num FROM player,state,color,team WHERE (PLAYER.TEAM_ID = TEAM.TEAM_ID) AND (TEAM.STATE_ID = STATE.STATE_ID) AND (TEAM.COLOR_ID = COLOR.COLOR_ID) AND (state.name =";
  query += " '" + team_state + "')" + " AND (" +" color.name = " + "'" + team_color +"');";
  work W(*C);
  result R = W.exec(query);
  cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" <<endl;
  for(auto c:R)
    cout <<c[0].as<string>() << ' '
	 <<c[1].as<string>() << ' '
	 <<c[2].as<string>() << endl;
}


void query5(connection *C, int num_wins)
{
  string query;
  query += "SELECT FIRST_NAME, LAST_NAME, NAME, WINS FROM PLAYER, TEAM WHERE (PLAYER.TEAM_ID = TEAM.TEAM_ID) AND (WINS > ";
  query += to_string(num_wins) + ") ;";
  work W(*C);
  result R = W.exec(query);

  cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;
  for(auto c:R)
    cout << c[0].as<string>() << ' '
	 << c[1].as<string>() << ' '
	 << c[2].as<string>() << ' '
	 << c[3].as<string>() << endl;
}
