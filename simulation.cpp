#include "simulation.h"

simulation::simulation()
{

}

#ifdef KUEYGOUYWEFGUI

int window[3];
int t;
int ID_number;

extern container P;

std::vector< std::vector< species > > world;
std::vector< std::vector< int > > viable;
std::vector<species> metaCommunity;
std::vector<species> speciesList;
std::vector<int> octaveNumbers;
std::vector< std::vector< int > > reproduction;
std::vector<int> indices;

LPVOID glutFonts[7] = {
  GLUT_BITMAP_9_BY_15,
  GLUT_BITMAP_8_BY_13,
  GLUT_BITMAP_TIMES_ROMAN_10,
  GLUT_BITMAP_TIMES_ROMAN_24,
  GLUT_BITMAP_HELVETICA_10,
  GLUT_BITMAP_HELVETICA_12,
  GLUT_BITMAP_HELVETICA_18
};

void updateWorld2()
{
  //for(int n = 0; n < P.N; ++n)
  //{

  int size = world.size();
  std::vector<species> filler(size);
  std::vector< std::vector< species > > newWorld(size,filler);

  std::vector<int> index = indices;

  std::random_shuffle(index.begin(), index.end());

  for(std::vector<int>::iterator i = index.begin(); i != index.end(); ++i)
    {
      int y = (*i) % world.size();
      int x = (int)( (*i) / world.size());

      //species at x,y dies and gets replaced
      //with prob 1-m it is from within the local community
      if(uniform() < P.m) //migration
        {
          species migrant = getSpeciesFromMetaCommunity();
          newWorld[x][y] = migrant;
        }
      else //no migration, local reproduction
        {
          int otherX = x; int otherY = y;
          if(P.dispersalType == "Square") findParent_Square(otherX,otherY,x,y); //square
          else if(P.dispersalType == "Circle") findParent_Circle(otherX,otherY,x,y); //circle
          //      else if(P.dispersalType == "Reflect") findParent_Reflect(otherX,otherY,x,y);
          else findParent_Circle(otherX,otherY,x,y);  //default


          newWorld[x][y] = world[otherX][otherY];
          if(uniform() < P.speciationRate) newWorld[x][y] = newSpecies();
        }
    }

  world = newWorld;
}




void updateWorld()
{
  for(int n = 0; n < (0.5*P.N); ++n)
    {
      //pick a random cell
      int x = random_number(P.sizeX);
      int y = random_number(P.sizeY);
      bool picked_viable = (bool)viable[x][y];
      while(picked_viable == false)
        {
          x = random_number(P.sizeX);
          y = random_number(P.sizeY);
          picked_viable = (bool)viable[x][y];
        }


      //species at x,y dies and gets replaced
      //with prob 1-m it is from within the local community
      if(uniform() < P.m) //migration
        {
          species migrant = getSpeciesFromMetaCommunity();
          world[x][y] = migrant;
        }
      else //no migration, local reproduction
        {
          int otherX = x; int otherY = y;
          if(P.dispersalType == "Square") findParent_Square(otherX,otherY,x,y); //square
          else if(P.dispersalType == "Circle") findParent_Circle(otherX,otherY,x,y); //circle
          //      else if(P.dispersalType == "Reflect") findParent_Reflect(otherX,otherY,x,y);
          else findParent_Circle(otherX,otherY,x,y);  //default


          world[x][y] = world[otherX][otherY];
          reproduction[otherX][otherY]++;
          //world[x][y] = world[otherX][otherY];


          if(uniform() < P.speciationRate) world[x][y] = newSpecies();
        }
    }
}


void Initialize()
{
  P.sizeX = viable.size();
  P.sizeY = viable[0].size();
  ID_number=0;

  for(int n = 0; n < P.N; ++n) indices.push_back(n);

  //std::cout << "starting Initialization Meta Community\n"; std::cout.flush();
  (*(P.FORM))->add_systemLog("starting Initialization Meta Community\r\n");
  initializeMetaCommunity();
  //  std::cout << "Meta community initialized\n"; std::cout.flush();
  (*(P.FORM))->add_systemLog("Meta community initialized\r\n");

  t = 0;

  std::vector<int> holder(P.sizeY,0);
  reproduction.resize(P.sizeX,holder); //initialize the reproduction counts

  std::vector<species> YY;
  YY.resize(P.sizeY,species());
  world.resize(P.sizeX,YY);

  species empty = species();
  empty.ID = -1;
  empty.C = color(0.0,0.0,0.0);

  //std::cout << "Intializing local community\n"; std::cout.flush();
  (*(P.FORM))->add_systemLog("Intializing local community\r\n");
  species monoDom = getSpeciesFromMetaCommunity();

  for(int x = 0; x < P.sizeX; ++x)
    {
      for(int y = 0; y < P.sizeY; ++y)
        {
          if(viable[x][y] == 1) {
              if(P.monoDominance == false) world[x][y] = getSpeciesFromMetaCommunity();
              else world[x][y] = monoDom;
            }
          else
            {
              world[x][y] = empty;
            }
        }
    }
  //sort species list
  std::sort(speciesList.begin(), speciesList.end(), sortSpeciesID);

  //std::cout << "Initializing octave numbers\n"; std::cout.flush();
  (*(P.FORM))->add_systemLog("Initializing octave numbers\r\n");
  for(std::size_t i = 0; i < 20; ++i)
    {
      octaveNumbers.push_back( pow(2.0,(int)i));
    }

  //std::cout << "Initialization complete\n"; std::cout.flush();
  graphMetaCommunity();
  (*(P.FORM))->add_systemLog("Initialization complete\r\n");
}

void reset_simulation(gui::Form1^ F)
{
  //(*(P.FORM))->add_systemLog("Resetting all variables");
  metaCommunity.clear();

  t = 0;
  reproduction.clear();
  world.clear();
  viable.clear();
  speciesList.clear();
  octaveNumbers.clear();

  (*(&F))->resetCharts();
}

void closeWindow()
{
  stop_simul();
  P.isRunning = false;

  (*(P.FORM))->resetText();
  //reset_simulation(*(P.FORM));
}

int go(gui::Form1^ F)
{
  P.FORM = &F;
  P.isRunning = true;

  set_seed(P.Seed);   //331

  char* dummyargs;
  dummyargs = (char*) malloc(120* sizeof(char));
  int dummyargc = 1;
  strcpy(dummyargs,"Do I really need to pass arguments?!");
  glutInit(&dummyargc,&dummyargs);

  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);

  int notImportant = 0;
  //toArray(viable,P.FileName,notImportant);
  std::vector<int> YY(P.size,1);
  viable.resize(P.size,YY);

  int area = P.size * P.size;
  P.N = (int)(0.25 * area);

  Initialize();

  //F->changeMetaText();


  P.showGraph = true;
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(viable.size(),viable[0].size());

  window[0] = glutCreateWindow("The Neutralizer");
  glutPositionWindow(300,300);
  glutReshapeWindow(400, 400);
  glutDisplayFunc(display);
  P.createdGraphWindow = true;


  glutIdleFunc(display);
  glutCloseFunc(closeWindow);
  glutMainLoop();

  return 0;
}




bool compareSpecies(const species& s1, const species&s2)
{
  if(s1.ID != s2.ID) return false;
  else
    {
      if(s1.C.r != s2.C.r) return false;
      else
        {
          if(s1.C.b != s2.C.b) return false;
          else
            {
              if(s1.C.g != s2.C.g) return false;
            }
        }
    }

  return true;
}

int countSpecies()
{
  //std::vector<int> counted;
  //counted.resize(speciesList.size(),0);
  int outputSize = 0;
  std::vector<species> found;
  for(std::vector< std::vector< species >>::iterator X = world.begin(); X != world.end(); ++X)
    {
      for(std::vector<species>::iterator Y = (*X).begin(); Y != (*X).end(); ++Y)
        {
          bool match = false;
          for(std::vector<species>::iterator s = found.begin(); s != found.end(); ++s)
            {
              if((*Y) == (*s))
                {
                  (*s).count++;
                  match = true;
                  break;
                }
            }

          if(match == false) //we have found no match, e.g. a "new" species
            {
              found.push_back((*Y));
              found.back().count = 1;
            }
        }
    }

  return found.size();
}

int countIndivs()
{
  int total = 0;
  for(std::vector< std::vector< species >>::iterator X = world.begin(); X != world.end(); ++X)
    {
      for(std::vector<species>::iterator Y = (*X).begin(); Y != (*X).end(); ++Y)
        {
          if((*Y).ID != -1) total++;
        }
    }
  return total;
}


void dispGraph()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //if(P.showReproduction == false)  rect();
  //else rect_repro();
  rect();
  glutSwapBuffers();
}

void updateSim()
{
  int numberSpec = -1;

  if(t%P.update == 0)
    {
      numberSpec = countSpecies();

      (*(P.FORM))->set_time(t);
      (*(P.FORM))->set_species(numberSpec);


      graphLocalCommunity();
    }

  if(t%10 == 0) {
      if(numberSpec < 0) numberSpec = countSpecies();
      (*(P.FORM))->updateSpeciesChart(t,numberSpec);
    }
  t++;
}



void rect()
{
  double stepX = 2.0f / P.sizeX;
  double stepY = 2.0f / P.sizeY;

  for(std::size_t X = 0; X < viable.size(); X++)
    {
      for(std::size_t Y = 0; Y < viable[X].size(); Y++)
        {

          //GLfloat x1 = -1.0f + 1.0f * X * stepX;
          double x1 = -1.0f + 1.0f * X * 2.0 / P.sizeX;
          double y1 = -1.0f + 1.0f * Y * 2.0 / P.sizeY;
          //1.0f - 1.0f * X * stepX;
          //GLfloat y1 = -1.0f + 1.0f * Y * stepY;//1.0f - 1.0f * Y * stepY;

          float grey = 100.0f / 255;
          color C_square = color(grey,grey,grey);

          if(viable[X][Y] == 1) C_square = world[X][Y].C;

          glColor3f(C_square.r,C_square.g,C_square.b);
          glRectf(x1,y1,x1 + 2.0 / P.sizeX,y1+2.0 / P.sizeY);

        }
    }
}


void display()
{
  if(P.isPaused == false) {
      updateWorld();
      updateSim();

      if(P.showGraph == true)
        {
          if(P.createdGraphWindow == false)
            {
              window[0] = glutCreateWindow("The Neutralizer");
              glutPositionWindow(1000,500);
              glutReshapeWindow(viable.size(), viable[0].size());

              if(t % P.update == 0) glutDisplayFunc(display);
            }
          glutSetWindow(window[0]);
          P.createdGraphWindow = true;
          if(t % P.update == 0) dispGraph();
        }
      else
        {
          if(P.createdGraphWindow == true)
            {
              //  glutDestroyWindow(window[0]);
              //  P.createdGraphWindow = false;
            }
        }
    }
}

color::color(const float &R, const float &G, const float &B)
{
  r = R;
  g = G;
  b = B;
}

color::color()
{
  r = (float)uniform();
  g = (float)uniform();
  b = (float)uniform();
}


species::species()
{
  ID = random_number(P.sizeX*P.sizeY);
  count = 0;
  C = color();
}

species::species(const double& f)
{
  ID = random_number(P.sizeX*P.sizeY);
  fraction = f;
  C = color();
}

void glutPrint(float x, float y, LPVOID font, char* text, float r, float g, float b, float a)
{
  if(!text || !strlen(text)) return;
  bool blending = false;
  if(glIsEnabled(GL_BLEND)) blending = true;
  glEnable(GL_BLEND);
  glColor4f(r,g,b,a);
  glRasterPos2f(x,y);
  while (*text) {
      glutBitmapCharacter(font, *text);
      text++;
    }
  if(!blending) glDisable(GL_BLEND);
}


void findParent_Square(int& otherX, int& otherY, const int& x, const int& y)
{
  otherX = random_number(2 * P.L + 1) - P.L;
  otherY = random_number(2 * P.L + 1) - P.L;

  otherX += x;
  otherY += y;

  bool suitable = true;
  if(otherX == x && otherY == y) suitable = false;
  else if(otherX < 0) suitable = false;
  else if(otherX >= P.sizeX) suitable = false;
  else if(otherY < 0) suitable = false;
  else if(otherY >= P.sizeY) suitable = false;
  else if(viable[otherX][otherY] == 0) suitable = false;

  while(suitable == false)
    {
      otherX = x + random_number(2 * P.L + 1) - P.L;
      otherY = y + random_number(2 * P.L + 1) - P.L;
      suitable = true;

      if(otherX == x && otherY == y) suitable = false;
      else if(otherX < 0) suitable = false;
      else if(otherX >= P.sizeX) suitable = false;
      else if(otherY < 0) suitable = false;
      else if(otherY >= P.sizeY) suitable = false;
      else if(viable[otherX][otherY] == 0) suitable = false;
    }
  return;
}

void findParent_Circle(int& other_X, int& other_Y, const int& x, const int& y)
{
  double range = P.L;
  if(range > P.sizeX || range > P.sizeY)
    {
      range = P.sizeX;
    }

  double distance = uniform() * range;
  double dir = uniform() * 2 * P.Pi;
  double pY = sin(dir) * distance;
  double pX = cos(dir) * distance;

  int otherX = (int)(0.5 + x + pX);
  int otherY = (int)(0.5 + y + pY);
  bool suitable = true;
  if(otherX == x && otherY == y) suitable = false;
  else if(otherX < 0) suitable = false;
  else if(otherX >= P.sizeX) suitable = false;
  else if(otherY < 0) suitable = false;
  else if(otherY >= P.sizeY) suitable = false;
  else if(viable[otherX][otherY] == 0) suitable = false;
  while(suitable == false)
    {
      distance = uniform() * range;
      dir = uniform() * 2 * P.Pi;
      pY = sin(dir) * distance;
      pX = cos(dir) * distance;
      otherX = (int)(0.5 + x + pX);
      otherY = (int)(0.5 + y + pY);

      suitable = true;
      if(otherX == x && otherY == y) suitable = false;
      else if(otherX < 0) suitable = false;
      else if(otherX >= P.sizeX) suitable = false;
      else if(otherY < 0) suitable = false;
      else if(otherY >= P.sizeY) suitable = false;
      else if(viable[otherX][otherY] == 0) suitable = false;
    }

  other_X = otherX;
  other_Y = otherY;
  return;
}

bool species::operator ==(const species &other)
{
  if(ID != other.ID) return false;
  else
    {
      if(C.r == other.C.r)
        {
          if(C.g == other.C.g)
            {
              if(C.b == other.C.b)
                {
                  return true;
                }
              else return false;
            }
          else return false;
        }
      else return false;
    }
}

int octave_sort(long ab_in) //adapted from James
{
  int result;
  if(ab_in <= 0)
    {
      result = 0;
    }
  else
    {
      long min = 1;
      long max = 2;
      result = 0;
      while(!((ab_in < max)&&(ab_in >= min)))
        {
          min = min*2;
          max = max*2;
          result ++;
        }
    }
  return result;
}

void graphMetaCommunity()
{
  if(P.graphedMeta == true) return;

  std::vector<int> counts;
  for(std::vector<species>::iterator s = metaCommunity.begin(); s != metaCommunity.end(); ++s)
    {
      if((*s).count > 0) counts.push_back((*s).count);
    }

  std::vector<double> logs (40,0);

  BOOST_FOREACH(int i, counts)
  {
    int octave = octave_sort(i);
    logs[octave]++;
  }


  bool empty = true;
  while(empty)
    {
      if(logs.back() == 0)
        {
          empty = true;
          logs.pop_back();  //cut off all empty parts of the vector
        }
      else
        {
          empty = false;
        }
    }
  P.graphedMeta = true;

  (*(P.FORM))->drawMetaChart(logs);
}

void graphLocalCommunity()
{
  std::vector<int> counts;
  std::vector<species> speciesCount;// = speciesList;
  for(std::vector< std::vector< species >>::iterator X = world.begin(); X != world.end(); ++X)
    {
      for(std::vector<species>::iterator Y = (*X).begin(); Y != (*X).end(); ++Y)
        {
          bool match = false;
          for(std::vector<species>::iterator s = speciesCount.begin(); s != speciesCount.end(); ++s)
            {
              if((*Y) == (*s))
                {
                  (*s).count++;
                  match = true;
                  break;
                }
            }

          if(match == false)
            {
              speciesCount.push_back((*Y));
              speciesCount.back().count = 1;
            }
        }
    }

  for(std::vector<species>::iterator S = speciesCount.begin(); S != speciesCount.end(); ++S)
    {
      if((*S).count > 0) counts.push_back((*S).count);
    }

  std::vector<double> logs (40,0);

  BOOST_FOREACH(int i, counts)
  {
    if(i == 1)
      {
        int a = 5;

      }
    int octave = octave_sort(i);
    logs[octave]++;
  }



  bool empty = true;
  while(empty)
    {
      if(logs.back() == 0)
        {
          empty = true;
          logs.pop_back();  //cut off all empty parts of the vector
        }
      else
        {
          empty = false;
        }
    }

  (*(P.FORM))->drawLocalChart(logs);

  //code to make rank abundance plot
  std::vector<double> percentages;
  int max = -1;
  BOOST_FOREACH(int i, counts)
  {
    if(i > max) max = i;
  }

  for(std::size_t i = 0; i < counts.size(); ++i)
    {
      percentages.push_back(  100.0 * counts[i] / max );
    }

  std::sort(percentages.begin(), percentages.end(),std::greater<double>());

  (*(P.FORM))->drawRankChart(percentages);
}


void stop_simul()
{
  glutLeaveMainLoop();
}

#endif
