#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
#include <set>
#include <queue>
#include "AnimatedSprite.h"

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
, FrightenedTimer(5)
, speed(0)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
    PathFollowing(deltaTime);
    this->mOwner->SetPosition(dir * speed * deltaTime + mOwner->GetPosition());
}


void GhostAI::Frighten()
{
    mState = Frightened;
    FrightenedTimer = 7.0f;
    speed = 65.0f;
    std::swap(mNextNode, mPrevNode);
    AnimatedSprite* as = mOwner->GetComponent<AnimatedSprite>();
    as->SetAnimation("scared0");
    mTargetNode = nullptr;
    mPath.clear();
}

void GhostAI::Start(PathNode* startNode)
{
    mOwner->SetPosition(startNode->GetPosition());
    mState = Scatter;
    mTargetNode = nullptr;
    mPrevNode = nullptr;
    mNextNode = nullptr;
    //Calculate path
    PathFinding(startNode, mGhost->GetScatterNode());
}

void GhostAI::Die()
{
    mState = Dead;
    speed = 125.0f;
    AnimatedSprite* as = mOwner->GetComponent<AnimatedSprite>();
    mTargetNode = mOwner->GetGame()->mGhostPen;
    if(dir.x > 0){
        as->SetAnimation("deadright");
    }
    else if(dir.x < 0){
        as->SetAnimation("deadleft");
    }
    else if(dir.y > 0){
        as->SetAnimation("deaddown");
    }
    else if(dir.y < 0){
        as->SetAnimation("deadup");
    }
    PathNode* t = mPrevNode;
    PathNode* temp = mNextNode;
    PathFinding(mNextNode, mOwner->GetGame()->mGhostPen);
    mPath.emplace_back(temp);
    mNextNode = temp;
    mPrevNode = t;
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	// Line from next node to subsequent on path
	SDL_RenderDrawLine(render,
		static_cast<int>(mNextNode->GetPosition().x),
		static_cast<int>(mNextNode->GetPosition().y),
		static_cast<int>(mPath.back()->GetPosition().x),
		static_cast<int>(mPath.back()->GetPosition().y));

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}

int contain(std::vector<std::pair<PathNode*, int> >& c , PathNode* n){
    for(int i = 0; i < (int)c.size(); ++i){
        if(c[i].first == n){
            return i;
        }
    }
    return -1;
}

float GhostAI::heur(PathNode* c, PathNode* t){
    PathNode* tleft = mOwner->GetGame()->mTunnelLeft;
    PathNode* tright = mOwner->GetGame()->mTunnelRight;
    
    float h1 = (c->GetPosition() - t->GetPosition()).Length();
    float h2 = (c->GetPosition() - tleft->GetPosition()).Length() + (tleft->GetPosition() - t->GetPosition()).Length();
    float h3 = (c->GetPosition() - tright->GetPosition()).Length() + (tright->GetPosition() - t->GetPosition()).Length();
    
    return std::min(h1, std::min(h2, h3));
}

PathNode* GhostAI::findNearest(std::vector<std::pair<PathNode*, int> >& c, PathNode* t)
{
    int index = 0;
    int d = c[0].second;
    d += heur(c[0].first, t);
    for(int i = 0; i < (int)c.size(); ++i){
        int g = c[i].second;
        int h = (int)heur(c[i].first, t);
        if(h + g < d){
            index = i;
            d = h + g;
        }
    }
    return c[index].first;
}


bool GhostAI::PathFinding(PathNode* startNode, PathNode* target)
{
    mPath.clear();
    if(startNode == target || mPrevNode == target){
        return false;
    }
    mTargetNode = target;
    
    
    PathNode* curr = startNode;
    std::set<PathNode*> closed;
    std::vector<std::pair<PathNode*, int> > open;
    closed.insert(startNode);
    open.emplace_back(std::make_pair(curr, 0));
    
    std::unordered_map<PathNode*, PathNode*> parents;
    
    while(curr != target){
        std::vector<PathNode*> adj = curr->mAdjacent;
        for(auto node : adj){
            if((curr == startNode && node == mPrevNode) || closed.find(node) != closed.end())
            {
                continue;
            }
            int index = contain(open, node);
            
            int distance = (curr->GetPosition() - node->GetPosition()).Length();
            
            if(curr->GetType() == PathNode::Tunnel && node->GetType() == PathNode::Tunnel)
            {
                distance = 0;
            }
            int g = open[contain(open, curr)].second + distance;
            if(index != -1){
                if(open[index].second > g){
                    parents[node] = curr;
                    open[index].second = g;
                }
            }
            else{
                parents[node] = curr;
                open.emplace_back(std::make_pair(node, g));
            }
        }
        
        if(open.empty()){
            break;
        }
        
        open.erase(open.begin() + contain(open, curr));
        closed.insert(curr);
        curr = findNearest(open, target);
    }
    
    if(curr == target){
        PathNode* temp = target;
        while(temp != startNode){
            mPath.emplace_back(temp);
            temp = parents[temp];
        }
        mNextNode = mPath.back();
        mPrevNode = startNode;
        return true;
    }
    else{
        return false;
    }
}

float getDist(PathNode* a, PathNode* b){
    float x = a->GetPosition().x - b->GetPosition().x;
    float y = a->GetPosition().y - b->GetPosition().y;
    return sqrt(x*x + y*y);
}

PathNode* GhostAI::FindNodeClosest(const Vector2& loc){
    const std::vector<PathNode*>& nodes = mOwner->GetGame()->mPathNodes;
    PathNode* x = nodes[0];
    for(PathNode* n : nodes){
        if(n->GetType() == PathNode::Ghost){
            continue;
        }
        if((n->GetPosition() - loc).Length() < (x->GetPosition() - loc).Length()){
            x = n;
        }
    }
    return x;
}


void GhostAI::ScatterMove(){
    
}

void GhostAI::SetAnim(){
    AnimatedSprite* as = mOwner->GetComponent<AnimatedSprite>();
    if(mState == Scatter){
        if(dir.x > 0){
            as->SetAnimation("right");
        }
        else if(dir.x < 0){
            as->SetAnimation("left");
        }
        else if(dir.y > 0){
            as->SetAnimation("down");
        }
        else if(dir.y < 0){
            as->SetAnimation("up");
        }
    }
    else if(mState == Dead){
        if(dir.x > 0){
            as->SetAnimation("deadright");
        }
        else if(dir.x < 0){
            as->SetAnimation("deadleft");
        }
        else if(dir.y > 0){
            as->SetAnimation("deaddown");
        }
        else if(dir.y < 0){
            as->SetAnimation("deadup");
        }
    }
    else if(mState == Chase){
        if(dir.x > 0){
            as->SetAnimation("right");
        }
        else if(dir.x < 0){
            as->SetAnimation("left");
        }
        else if(dir.y > 0){
            as->SetAnimation("down");
        }
        else if(dir.y < 0){
            as->SetAnimation("up");
        }
    }
}

void GhostAI::Teleport(PathNode *target){
    if(mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())){
        if(mNextNode->GetType() == PathNode::Tunnel){
            if(mNextNode == mOwner->GetGame()->mTunnelRight){
                mOwner->SetPosition(mOwner->GetGame()->mTunnelLeft->GetPosition());
                mNextNode = mOwner->GetGame()->mTunnelLeft;
                mPrevNode = mNextNode;
            }
            else{
                mOwner->SetPosition(mOwner->GetGame()->mTunnelRight->GetPosition());
                mNextNode = mOwner->GetGame()->mTunnelRight;
                mPrevNode = mNextNode;
            }
            PathFinding(mNextNode, target);
        }
    }
}

void GhostAI::PathFollowing(float deltaTime){
    FrightenedTimer -= deltaTime;
    
    if(mState == Scatter){
        if(FrightenedTimer > 0){
            speed = 90.0f;
            if(mNextNode != nullptr && mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>()))
            {
                Teleport(mGhost->GetScatterNode());
                if(!mPath.empty()){
                    mPath.pop_back();
                }
                
                if(!mPath.empty()){
                    mPrevNode = mNextNode;
                    mNextNode = mPath.back();
                }
                else{
                    if(!PathFinding(mNextNode, mGhost->GetScatterNode())){
                        FailSafe();
                    }
                }
            }
            if(mNextNode != nullptr){
                SetDirection();
                SetAnim();
            }
        }
        else{
            mState = Chase;
            mPath.clear();
            FrightenedTimer = 20.0f;
        }
    }
    else if(mState == Frightened){
        if(FrightenedTimer > 0){
            if(FrightenedTimer < 2.0f){
                mOwner->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
            }
            if(mNextNode != nullptr && mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>()))
            {
                const std::vector<PathNode*>& adj = mNextNode->mAdjacent;
                PathNode* x = adj[Random::GetIntRange(0, static_cast<int>(adj.size() - 1))];
                while(x == mPrevNode || x->GetType() == PathNode::Ghost){
                    x = adj[Random::GetIntRange(0, static_cast<int>(adj.size() - 1))];
                }
                mPrevNode = mNextNode;
                mNextNode = x;
            }
            if(mNextNode != nullptr){
                SetDirection();
            }
        }
        else{
            mState = Scatter;
            mTargetNode = mGhost->GetScatterNode();
            FrightenedTimer = 5.0f;
        }
    }//End of Frightened
    else if(mState == Dead){
        if(mNextNode != nullptr && mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>()))
        {
            
            if(!mPath.empty()){
                mPath.pop_back();
            }
            
            if(!mPath.empty()){
                mPrevNode = mNextNode;
                mNextNode = mPath.back();
            }
            else{
                Start(mNextNode);
            }
            
            if(mNextNode != nullptr){
                SetDirection();
                SetAnim();
            }
        }
    }
    else if(mState == Chase){
        if(FrightenedTimer > 0){
            CollisionComponent* cc = mOwner->GetComponent<CollisionComponent>();
            if(mGhost->GetType() == Ghost::Type::Blinky){
                if(mNextNode != nullptr && cc->Intersect(mNextNode->GetComponent<CollisionComponent>()))
                {
                    mTargetNode = mOwner->GetGame()->mPlayer->GetPrevNode();
                    Teleport(mTargetNode);
                    if(!PathFinding(mNextNode, mOwner->GetGame()->mPlayer->GetPrevNode())){
                        FailSafe();
                    }
                    if(mNextNode->GetType() == PathNode::Ghost){
                        const std::vector<PathNode*>& adj = mPrevNode->mAdjacent;
                        PathNode* x = adj[Random::GetIntRange(0, static_cast<int>(adj.size() - 1))];
                        while(x == mPrevNode || x->GetType() == PathNode::Ghost){
                            x = adj[Random::GetIntRange(0, static_cast<int>(adj.size() - 1))];
                        }
                        mNextNode = x;
                    }
                }
            }
            else if(mGhost->GetType() == Ghost::Type::Pinky){
                PathNode* x = FindNodeClosest(mOwner->GetGame()->mPlayer->GetPointInFrontOf(80.0f));
                if(mNextNode != nullptr && cc->Intersect(mNextNode->GetComponent<CollisionComponent>()))
                {
                    mTargetNode = x;
                    Teleport(mTargetNode);
                    if(!PathFinding(mNextNode, x)){
                        FailSafe();
                    }
                    if(mNextNode->GetType() == PathNode::Ghost){
                        const std::vector<PathNode*>& adj = mPrevNode->mAdjacent;
                        PathNode* x = adj[Random::GetIntRange(0, static_cast<int>(adj.size() - 1))];
                        while(x == mPrevNode || x->GetType() == PathNode::Ghost){
                            x = adj[Random::GetIntRange(0, static_cast<int>(adj.size() - 1))];
                        }
                        mNextNode = x;
                    }
                }
            }
            else if(mGhost->GetType() == Ghost::Type::Inky){
                Vector2 x = mOwner->GetGame()->mPlayer->GetPointInFrontOf(40.0f);
                x -= mOwner->GetGame()->mGhosts[0]->GetPosition();
                x = x * 2 + mOwner->GetGame()->mGhosts[0]->GetPosition();
                PathNode* dist = FindNodeClosest(x);
                if(mNextNode != nullptr && mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>()))
                {
                    mTargetNode = dist;
                    Teleport(mTargetNode);
                    if(!PathFinding(mNextNode, dist)){
                        FailSafe();
                    }
                }
            }
            else{
                float distance = (mOwner->GetGame()->mPlayer->GetPosition() - mOwner->GetPosition()).Length();
                if(distance > 150.0f){
                    if(mNextNode != nullptr && mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>()))
                    {
                        mTargetNode = mOwner->GetGame()->mPlayer->GetPrevNode();
                        Teleport(mTargetNode);
                        if(!PathFinding(mNextNode, mOwner->GetGame()->mPlayer->GetPrevNode())){
                            FailSafe();
                        }
                    }
                }
                else{
                    if(mNextNode != nullptr && mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>()))
                    {
                        mTargetNode = mGhost->GetScatterNode();
                        Teleport(mTargetNode);
                        if(!PathFinding(mNextNode, mGhost->GetScatterNode())){
                            FailSafe();
                        }
                    }
                }
            }
            if(mNextNode != nullptr){
                SetDirection();
                SetAnim();
            }
        }
        else{
            FrightenedTimer = 5.0f;
            mTargetNode = mGhost->GetScatterNode();
            mPath.clear();
            mState = Scatter;
        }
    }
}

void GhostAI::SetDirection(){
    float x_dif = mNextNode->GetPosition().x - mPrevNode->GetPosition().x;
    float y_dif = mNextNode->GetPosition().y - mPrevNode->GetPosition().y;
    Vector2 dirt = Vector2(x_dif, y_dif);
    dir = Vector2(x_dif / dirt.Length(), y_dif / dirt.Length());
}


void GhostAI::FailSafe(){
    mPath.clear();
    PathNode* x = nullptr;
    const std::vector<PathNode*>& adj = mNextNode->mAdjacent;
    for(PathNode* p : adj){
        if(p == mPrevNode || p->GetType() == PathNode::Ghost){
            continue;
        }
        if(x == nullptr || getDist(p, mTargetNode) < getDist(x, mTargetNode)){
            x = p;
        }
    }
    mPrevNode = mNextNode;
    mNextNode = x;
}
