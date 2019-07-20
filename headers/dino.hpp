class Dino{
private:
  int y;
  int velocityY, thrust;
  int originalY;
  int runTimer;
  int textureIndex;
  bool duck;

  bool dead;

  vector<Texture> textures;
  Texture dinoTexture;
  Texture dinoRun1;
  Texture dinoRun2;
  Texture dinoJump;
  Texture dinoDead;
  Texture dinoDuck1;
  Texture dinoDuck2;
private:
  void animate(){
    cout<<dead<<endl;
    if(!dead){
      if (y == originalY){//on ground
        if(runTimer == 5){//change interval
          if(!duck){//running
            switch(textureIndex){
              case 0: //start
                textureIndex = 1;
                break;
              case 1: //run
                textureIndex = 2;
                break;
              case 2: //run
                textureIndex = 1;
                break;
              case 3: //run
                textureIndex = 1;
                break;
              default:
                textureIndex = 1;
                break;
            }
          }else{//duck
            switch(textureIndex){
              case 5:
                textureIndex = 6;
                break;
              case 6:
                textureIndex = 5;
                break;
              default:
                textureIndex = 6;
                break;
            }
          }

          runTimer = 0;
        }
      }else{ //jump
        textureIndex = 3;
        runTimer = 0;
      }
    }else{
      textureIndex = 4;
    }
    runTimer++;
  }
public:
  inline getY(){return y;}

  Dino(int posX, int posY){
    y=posY;
    velocityY = 0;
    thrust = 22;
    originalY=posY;
    runTimer = 0;
    textureIndex = 0;
    duck = false;
    dead = false;

    dinoTexture.loadFromFile("art/dino.png");
    dinoRun1.loadFromFile("art/dinoRun1.png");
    dinoRun2.loadFromFile("art/dinoRun2.png");
    dinoJump.loadFromFile("art/dinoJump.png");
    dinoDead.loadFromFile("art/dinoDead.png");
    dinoDuck1.loadFromFile("art/dinoDuck1.png");
    dinoDuck2.loadFromFile("art/dinoDuck2.png");

    textures.push_back(dinoTexture);
    textures.push_back(dinoRun1);
    textures.push_back(dinoRun2);
    textures.push_back(dinoJump);
    textures.push_back(dinoDead);
    textures.push_back(dinoDuck1);
    textures.push_back(dinoDuck2);
  }
  void Duck(){duck = true;}

  inline void unduck(){duck = false;}
  inline bool isDuck(){return duck;}
  inline void Reset(){y=originalY;unduck();}

  void Update(int gravity, bool quit){
    dead = quit ? true : false;
    if (!quit){
      animate();
      if(y < originalY){//above ground
        velocityY += gravity;
      }else if(y > originalY){//below ground
        y = originalY;
      }else{//on ground
        if(velocityY == thrust){
          velocityY = 0;
        }
      }

      y += velocityY;
    }else{
      animate();
    }
  }
  void Jump(){
    if (y == originalY){
      velocityY -= thrust;
    }
  }

  void Draw(RenderWindow& window){
    if(duck && y==originalY){
      RectangleShape dinoShape(Vector2f(120.0f,44.0f));

      dinoShape.setTexture(&textures[textureIndex]);
      dinoShape.setOrigin(60.0f, 22.0f);

      dinoShape.setPosition(100,y+30);

      dinoShape.setOutlineColor(Color::Black);
      dinoShape.setOutlineThickness(2.0f);

      window.draw(dinoShape);
    }else{
      RectangleShape dinoShape(Vector2f(90.0f,110.0f));

      dinoShape.setTexture(&textures[textureIndex]);
      dinoShape.setOrigin(45.0f, 55.0f);

      dinoShape.setPosition(100,y);

      // dinoShape.setOutlineColor(Color::Black);
      // dinoShape.setOutlineThickness(2.0f);

      window.draw(dinoShape);
    }

  }

};
