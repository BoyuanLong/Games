//
//  Key.hpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Key_hpp
#define Key_hpp

#include "Actor.h"
class Game;

namespace KEY {
const int SizeX = 16;
const int SizeY = 32;
}

class Key : public Actor{
public:
    Key(Game* game);
};

#endif /* Key_hpp */
