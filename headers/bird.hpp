class Bird{
private:
  int x,y;

  int texturesIndex;
  vector<Texture> textures;
  Texture bird1;
  Texture bird2;

  int flyTimer;
public:
  Bird(){
    x=1500;
    int randNum = rand()%3;
    switch (randNum) {
      case 0:
        y=600;
        break;
      case 1:
        y=550;
        break;
      case 2:
        y=510;
        break;
    }

    texturesIndex = 1;
    bird1.loadFromFile("art/bird.png");
    bird2.loadFromFile("art/bird2.png");
    textures.push_back(bird1);
    textures.push_back(bird2);

    flyTimer = 0;
  }

  inline getX(){return x;}
  inline getY(){return y;}

  void Animate(){
    if (flyTimer==20){
      switch(texturesIndex){
        case 0:
          texturesIndex = 1;
          break;
        case 1:
          texturesIndex = 0;
          break;
      }
      flyTimer = 0;
    }
    flyTimer++;
  }

  void Update(float speed){
   x -= speed;
   Animate();
  }

  void Draw(RenderWindow& window){
    RectangleShape birdShape(Vector2f(90.0f,100.0f));
    birdShape.setOrigin(45.0f,50.0f);
    birdShape.setPosition(x,y);
    birdShape.setTexture(&textures[texturesIndex]);

    birdShape.setOutlineColor(Color::Black);
    birdShape.setOutlineThickness(2.0f);

    window.draw(birdShape);
  }
};
