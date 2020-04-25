#include "player_game.h"


Player_Game::Player_Game() 
{
  
   boost::uuids::random_generator generator;
   boost::uuids::uuid id = generator();
   _id = id;

}

void Player_Game::setName(std::string name) { _name = name; }

std::vector<std::string> Player_Game::setHand(nlohmann::json cards, int cardNum) 
{
	std::vector<std::string> files;
	int num;
	std::string s;
	Suit suit;
	for(int i = 1; i <= cardNum; i++) 	
	{
		std::stringstream ss;
		std::string in = std::to_string(i);
		std::string cStr = cards[in];
		ss << cStr;
		ss >> num;
		ss >> s;
		if(s == "S")
			suit = S;
		else if(s == "C")
			suit = C;
		else if(s == "D")
			suit = D;
		else
			suit = H;

		Card c(num, suit);
		std::cout << c.card_to_string() << std::endl;
		_hand.addCard(c);	
	}
	std::cout << "setting hand" << std::endl;

	std::vector<Card> hand = _hand.getHand();
	for(auto card : hand) {
		files.push_back(card.card_to_filename());
		std::cout << card.card_to_filename() << std::endl;
	}

	return files;
}
		
	
//Changed this function a little to make it easier to use in mainwin
chat_message Player_Game::move_j(std::string play, int cards_requested, int current_bet)
{
  nlohmann::json to_dealer;
  chat_message uuid;
  std::string json_str;
	
  to_dealer["uuid"] = this->_id;
  to_dealer["name"] = this->_name;
  to_dealer["event"] = play;        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = cards_requested;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = current_bet;
  //to_dealer["total_bet"] = this->total_bet;
  //to_dealer["chat"] = std::string(chat);

  json_str = to_dealer.dump();

  uuid.body_length(std::strlen(json_str.c_str()));
  std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
  uuid.encode_header();

  if(play == "bet" || play == "ante" || play == "raise") 
  {
    int blue = current_bet/25;
	std::cout << blue << std::endl;
	current_bet -= 25*blue;
    int green = current_bet/5;
    current_bet -= 5*green;
    int red = current_bet;
	std::cout << current_bet << std::endl;
	_stack.remove_chips(green,red,blue);
  }

  return uuid;

}

chat_message Player_Game::exchange_j(std::string play, int cards_requested, std::vector<int> cards) 
{
  _hand.modify_hand(cards);

  nlohmann::json to_dealer;
  chat_message uuid;
  std::string json_str;


  to_dealer["uuid"] = this->_id;
  to_dealer["name"] = this->_name;
  to_dealer["event"] = "request_cards";        // "stand","hit","fold","raise","join","request_cards"
  to_dealer["cards_requested"] = cards_requested;    // optional, number of cards requested, 1 to 5
  to_dealer["current_bet"] = 0;
  //to_dealer["total_bet"] = this->total_bet;
  //to_dealer["chat"] = std::string(chat);

  json_str = to_dealer.dump();

  uuid.body_length(std::strlen(json_str.c_str()));
  std::memcpy(uuid.body(), json_str.c_str(), uuid.body_length());
  uuid.encode_header();

  return uuid;
}






