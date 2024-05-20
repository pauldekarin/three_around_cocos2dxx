//
//  GameLayer.cpp
//  HelloCpp
//
//  Created by Bimba on 2024-04-28.
//

#include "GameLayer.hpp"

USING_NS_CC;


void GameLayer::runWith(int rows, int cols, int colours){
    clear();
    prepare(rows, cols, colours);
    generateMap(0);
    fillMap();
}

void GameLayer::finish(){
    std::stringstream buffer;
    buffer
        << "На этом все!"
        << std::endl
        << "Осталось: " << _blocks_container->getChildrenCount()
    << " / " << _rows * _cols;
    Label *label = Label::createWithTTF(buffer.str().c_str(), "fonts/arial.ttf", 36);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(getContentSize()/2);
    label->setName("finish_view");
    
    this->addChild(label,999);
}


void GameLayer::updateMap(){
    for(int col = 0; col < _cols; col++){
        for(int row = 1; row < _rows; row++){
            if(_map[row][col] != 0 && _map[row - 1][col] == 0){
                int k;
                for(k = row; k > 0 && _map[k - 1][col] == 0; k--);
                _map[k][col] = _map[row][col];
                _map[row][col] = 0;
               
            }
        }
    }
}

bool GameLayer::hasMoves(){
    for(int row = 0; row < _rows; row++){
        for(int col = 0; col < _cols; col++){
            if(_map[row][col] != 0 && hasEnoughNeighbors(row, col))
                return true;
        }
    }
    return false;
}

BlockNode *GameLayer::getBlockByIndex(int row, int col){
    for(Node *node : _blocks_container->getChildren()){
        try{
            BlockNode *block_node = dynamic_cast<BlockNode*>(node);
            if((int)std::round(block_node->getPositionX() / _edge_size) == col &&
               (int)std::round(block_node->getPositionY() / _edge_size) == row){
                return block_node;
            }
        }catch(std::bad_cast &err){
            break;
        }
    }
    return nullptr;
}

bool GameLayer::hasEnoughNeighbors(int row, int col){
    GLbyte c = 0;
    if(row < _rows - 1)
        c += (_map.at(row).at(col) == _map.at(row + 1).at(col)) ? 1 : 0;
    if(row > 0)
        c += (_map.at(row).at(col) == _map.at(row - 1).at(col)) ? 1 : 0;
    if(col < _cols -1)
        c += (_map.at(row).at(col) == _map.at(row).at(col + 1)) ? 1 : 0;
    if(col > 0)
        c += (_map.at(row).at(col) == _map.at(row).at(col - 1)) ? 1 : 0;
    return c >= 3;
}

void GameLayer::destroyBlockAndNeighbors(int row, int col, int c){
    BlockNode *block_node;
    if((block_node = getBlockByIndex(row, col)) == nullptr ||
        _map.at(row).at(col) == 0) return;
    
    _blocks_container->removeChild(block_node);
    _map.at(row).at(col) = 0;
    
    if(row < _rows - 1 &&  _map.at(row + 1).at(col) == c) destroyBlockAndNeighbors(row + 1, col, c);
    if(row > 0 && _map.at(row - 1).at(col) == c) destroyBlockAndNeighbors(row - 1, col, c);
    if(col < _cols - 1 && _map.at(row).at(col + 1) == c) destroyBlockAndNeighbors(row, col + 1, c);
    if(col > 0 && _map.at(row).at(col - 1) == c) destroyBlockAndNeighbors(row, col - 1, c);
}
bool GameLayer::init(){
    if(!Node::init()){
        return false;
    }
    
    _background_color = LayerColor::create();
    _blocks_container = Node::create();
    _edge_node = Node::create();
        
    addChild(_background_color,1);
    addChild(_blocks_container,2);
    addChild(_edge_node,2);
    
    EventListenerMouse *mouse_listener = EventListenerMouse::create();
    mouse_listener->onMouseUp = [=](Event *event){
        
        try{
            EventMouse *mouseEvent = dynamic_cast<EventMouse*>(event);
            Point mousePos = mouseEvent->getLocationInView() - _blocks_container->getPosition();
            if(!_blocks_container->getBoundingBox().containsPoint(mouseEvent->getLocationInView())) return;
            event->stopPropagation();
            
            int row = (int)std::floor((float)(mousePos.y - getPositionY()) / _edge_size),
                col = (int)std::floor((float)(mousePos.x - getPositionX()) / _edge_size);
          
            if(_map[row][col] == 0) return;
            
            if(hasEnoughNeighbors(row, col)){
                destroyBlockAndNeighbors(row, col, _map.at(row).at(col));
                updateMap();
                if(!hasMoves()){
                    finish();
                }
            }
        }catch(std::exception &er){
            CCLOG(er.what());
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouse_listener, this);
    
    return true;
}

int lerpi(int __lh, int __rh, float __f){
    return (int)std::round((1 - __f) * __lh + __f * __rh);
}
void GameLayer::generateMap(int seed){
    srand((unsigned int)time(NULL));
    int i,j;
    
    _blocks_container->removeAllChildren();
    _map.clear();
    _map.resize(_rows);
    for(std::vector<GLuint> &l : _map) l.resize(_cols);
    
    std::vector<Color4B> colours_container;
    colours_container.resize(_colours);
    for(i = 0; i < _colours; i++){
        cocos2d::Color4B colour = cocos2d::Color4B(rand() % 256,rand() % 256,rand() % 256,255);
        colours_container[i] = colour;
    }
                    
    for(i = 0; i < _rows; i++){
        for(j = 0; j < _cols; j++){
            cocos2d::Color4B colour = colours_container.at(rand() % lerpi(_colours, 1, clampf(seed, 0, 10) / 10));
            _map[i][j] = (colour.r << 24) | (colour.g << 16) | (colour.b << 8) | (colour.a);
        }
    }
    
    if(!hasMoves()) generateMap(++seed);
}

void GameLayer::clear(){
    _blocks_container->removeAllChildren();
    try{
        removeChild(getChildByName<Label*>("finish_view"));
    }catch(std::exception &exc){
        CCLOG(exc.what());
    }
}

void GameLayer::fillMap(){
    Size avaliable_size = getContentSize();
    _edge_size = std::floor(MIN(avaliable_size.width / _cols, avaliable_size.height / _rows));
    Size block_size = Size(_edge_size, _edge_size);
    Size game_view_size = Size(_edge_size * _cols, _edge_size * _rows);
    
    _blocks_container->setContentSize(game_view_size);
    
    for(int row = 0; row < _rows; row++){
        for(int col = 0; col < _cols; col++){
            GLuint gl_color = _map.at(row).at(col);
            
            Color4B cc_color = Color4B((gl_color >> 24) & 0xff,
                                       (gl_color >> 16) & 0xff,
                                       (gl_color >> 8) & 0xff,
                                       (gl_color >> 0) & 0xff);
            BlockNode *block_node = BlockNode::create(block_size, Color4F(cc_color));
            block_node->setPosition(Vec2(col * _edge_size, row * _edge_size));
            
            _blocks_container->addChild(block_node);
        }
    }
    PhysicsBody *edge_body = PhysicsBody::createEdgeBox(game_view_size , PhysicsMaterial(),0);
   
    
    _edge_node->setContentSize(game_view_size);
    _edge_node->setPhysicsBody(edge_body);
    
    _edge_node->setPosition((avaliable_size - game_view_size)/2);
    _blocks_container->setPosition((avaliable_size - game_view_size)/2);
}
void GameLayer::prepare(int rows, int cols, int colours){
    CCASSERT(rows != 0, "Game::update() 'rows' must be greater");
    CCASSERT(cols != 0, "Game::update() 'cols' must be greater");
    
    _rows = rows;
    _cols = cols;
    _colours = colours;
}

void GameLayer::setBackground(cocos2d::Color3B color){
    _background_color->setColor(color);
    _background_color->setOpacity(255);
}
