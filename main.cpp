#include <GLUT/glut.h>
#include <cmath>
#include <ctime>
#define CTRL_COUNT 100
#define PI 3.14159265359
#define MAX_RAINDROPS 600
#define MAX_SPLASHES 200

float kiteOffset = 0.0f;
float kiteDir    =  1.0f;
float wheelAngle = 0.0f;
float angle =0.0f;
float targetAngle   = 0.0f;
bool  rotating      = false;
const float speed1   = 3.0f;
float _moveA = 0.0f;
float _moveB = 0.0f;
float _moveC = 0.0f;
float speed = 0.06f;
float planeX = 0.0f;
float planeSpeed=0.2f*2;


static float wavePhase    = 0.0f;
static const float waveSpeed = 0.05f;   // how fast the ripples move
static const float waveAmp   = 2.0f;    // how tall the ripples are
static const float waveFreq  = 0.2f;    // how many ripples across the span
void init() {
    // clear to white
    glClearColor(0.3176f, 0.5647f, 0.6588f, 1.0f);


    // set up an orthographic projection once
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 80.0);

    glMatrixMode(GL_MODELVIEW);
}

inline float fakeRand(int seed) {
    return fmod(sin(seed * 12.9898f) * 43758.5453f, 1.0f); // returns [0.0, 1.0)
}


typedef struct {
    float x, y;
    float targetY;
} RainDrop;

typedef struct {
    float x, y;
    float radius;
    int active;
    float alpha;
} Splash;

RainDrop rain[MAX_RAINDROPS];
Splash splashes[MAX_SPLASHES];
float rainSpeed = 0.6f;

void initRain() {
    for (int i = 0; i < MAX_RAINDROPS; i++) {
        rain[i].x = static_cast<float>(rand()) / RAND_MAX * 100.0f;  // full range float
        rain[i].y = 100.0f + rand() % 50;
        rain[i].targetY = rand() % 61;
    }
    for (int i = 0; i < MAX_SPLASHES; i++) {
        splashes[i].active = 0;
    }
}


void addSplash(float x, float y) {
    for (int i = 0; i < MAX_SPLASHES; i++) {
        if (!splashes[i].active) {
            splashes[i].x = x;
            splashes[i].y = y;
            splashes[i].radius = 0.01f;
            splashes[i].alpha = 1.0f;
            splashes[i].active = 1;
            break;
        }
    }
}

void drawRain() {
    glColor3f(0.7f, 0.8f, 1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < MAX_RAINDROPS; i++) {
        glVertex2f(rain[i].x, rain[i].y);
        glVertex2f(rain[i].x, rain[i].y - 2.0f);
    }
    glEnd();
}

void drawSplashes() {
    for (int i = 0; i < MAX_SPLASHES; i++) {
        if (splashes[i].active) {
            glColor4f(1.0f, 1.0f, 1.0f, splashes[i].alpha);
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < 12; j++) {
                float angle = 2 * PI * j / 12;
                float dx = splashes[i].radius * cos(angle);
                float dy = splashes[i].radius * sin(angle);
                glVertex2f(splashes[i].x + dx, splashes[i].y + dy);
            }
            glEnd();
        }
    }
}


void drawWindmill(float x, float y) {
    // Base (tower)
    glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(x - 1.0f, y);
        glVertex2f(x + 1.0f, y);
        glVertex2f(x + 0.5f, y + 10.0f);
        glVertex2f(x - 0.5f, y + 10.0f);
    glEnd();

    // Hub center
    float cx = x;
    float cy = y + 10.0f;

    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON); // small circle as hub
    for (int i = 0; i < 360; i += 10) {
        float rad = i * M_PI / 180.0;
        glVertex2f(cx + cos(rad) * 0.5f, cy + sin(rad) * 0.5f);
    }
    glEnd();

    // Blades
    glPushMatrix();
        glTranslatef(cx, cy, 0.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 3; i++) {
            glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
            glBegin(GL_TRIANGLES);
                glVertex2f(0.0f, 0.0f);
                glVertex2f(0.5f, 4.0f);
                glVertex2f(-0.5f, 4.0f);
            glEnd();
        }
    glPopMatrix();
}

void drawStars(int count) {
    glPointSize(2.0f); // Star size
    glBegin(GL_POINTS);
    for (int i = 0; i < count; ++i) {
        float x = fakeRand(i * 3) * 100.0f;         // x in [0, 100]
        float y = 60.0f + fakeRand(i * 5) * 20.0f;  // y in [60, 80]

        // Slight variation in star color
        float r = 0.9f + fakeRand(i * 7) * 0.1f;    // 0.9 to 1.0
        float g = r;
        float b = r * 0.9f;

        glColor3f(r, g, b);
        glVertex2f(x, y);
    }
    glEnd();
}



void nboat(){
    glBegin(GL_QUADS);
      glColor3ub(139, 69, 19);
      glVertex2f(22.0f,  0.5f);
      glVertex2f(21.0f,  1.5f);
      glVertex2f(17.5f,  1.5f);
      glVertex2f(18.0f,  0.5f);
    glEnd();

    // 2) Black stripe
    glBegin(GL_QUADS);
      glColor3ub(0, 0, 0);
      glVertex2f(22.0f,  0.0f);
      glVertex2f(22.0f,  0.5f);
      glVertex2f(17.0f,  0.5f);
      glVertex2f(17.0f,  0.0f);
    glEnd();

    

    

    // 5) Stern triangle
    glBegin(GL_TRIANGLES);
      glColor3ub(0, 0, 0);
      glVertex2f(17.0f,  0.0f);
      glVertex2f(17.0f,  0.5f);
      glVertex2f(15.5f,  0.8f);
    glEnd();

    // 6) Bow triangle
    glBegin(GL_TRIANGLES);
      glColor3ub(0, 0, 0);
      glVertex2f(22.0f,  0.5f);
      glVertex2f(22.0f,  0.0f);
      glVertex2f(23.5f,  0.8f);
    glEnd();

    // 7) Deck-detail triangle
    glBegin(GL_TRIANGLES);
      glColor3ub(0, 0, 0);
      glVertex2f(17.0f,  0.5f);
      glVertex2f(18.0f,  0.5f);
      glVertex2f(17.5f,  1.5f);
    glEnd();
}



void ndrawPlane() {
    glPushMatrix();
    glTranslatef(planeX, 65.0f, 0.0f);
    glScalef(-16.0, 30.0, 0);
    
    // ================= Plane Body =================
    glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(-0.4214f, 0.2920f);
    glVertex2f(-0.8376f, 0.2912f);
    glVertex2f(-0.9320f, 0.2881f);
    glVertex2f(-0.9289f, 0.2556f);
    glVertex2f(-0.9041f, 0.2378f);
    glVertex2f(-0.8732f, 0.2278f);
    glVertex2f(-0.8453f, 0.2193f);
    glVertex2f(-0.8144f, 0.2108f);
    glVertex2f(-0.7842f, 0.2069f);
    glVertex2f(-0.7440f, 0.2077f);
    glVertex2f(-0.3897f, 0.2092f);
    glVertex2f(-0.3603f, 0.2131f);
    glEnd();
    
    // ================= Plane Head =================
    glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(-0.4214f, 0.2920f);
    glVertex2f(-0.4044f, 0.2893f);
    glVertex2f(-0.3832f, 0.2803f);
    glVertex2f(-0.3546f, 0.2671f);
    glVertex2f(-0.3381f, 0.2622f);
    glVertex2f(-0.3238f, 0.2573f);
    glVertex2f(-0.3177f, 0.2518f);
    glVertex2f(-0.3122f, 0.2463f);
    glVertex2f(-0.3100f, 0.2400f);
    glVertex2f(-0.3098f, 0.2357f);
    glVertex2f(-0.3100f, 0.2300f);
    glVertex2f(-0.3138f, 0.2259f);
    glVertex2f(-0.3204f, 0.2228f);
    glVertex2f(-0.3281f, 0.2191f);
    glVertex2f(-0.3379f, 0.2157f);
    glVertex2f(-0.3500f, 0.2128f);
    glVertex2f(-0.3603f, 0.2131f);
    glEnd();
    
    // ================= Head Windows =================
    glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-0.3819f, 0.2759f);
    glVertex2f(-0.3881f, 0.2729f);
    glVertex2f(-0.3772f, 0.2609f);
    glVertex2f(-0.3705f, 0.2634f);
    glVertex2f(-0.3661f, 0.2658f);
    glVertex2f(-0.3609f, 0.2696f);
    glVertex2f(-0.3787f, 0.2782f);
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-0.3989f, 0.2687f);
    glVertex2f(-0.4099f, 0.2680f);
    glVertex2f(-0.4085f, 0.2627f);
    glVertex2f(-0.4031f, 0.2556f);
    glVertex2f(-0.3926f, 0.2562f);
    glVertex2f(-0.3805f, 0.2589f);
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-0.4128f, 0.2603f);
    glVertex2f(-0.4148f, 0.2668f);
    glVertex2f(-0.4318f, 0.2669f);
    glVertex2f(-0.4323f, 0.2604f);
    glVertex2f(-0.4296f, 0.2515f);
    glVertex2f(-0.4078f, 0.2526f);
    glEnd();
    
    // ================= Front Door =================
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(-0.4540f, 0.2650f);
    glVertex2f(-0.4772f, 0.2641f);
    glVertex2f(-0.4757f, 0.2159f);
    glVertex2f(-0.4517f, 0.2157f);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex2f(-0.4540f, 0.2650f);
    glVertex2f(-0.4772f, 0.2641f);
    glVertex2f(-0.4757f, 0.2159f);
    glVertex2f(-0.4517f, 0.2157f);
    glEnd();
    
    // Door Handle
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-0.4696f, 0.2426f);
    glVertex2f(-0.4728f, 0.2424f);
    glVertex2f(-0.4726f, 0.2379f);
    glVertex2f(-0.4693f, 0.2381f);
    glEnd();
    
    
    // ================= Wings =================
    // Upper Wing
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(-0.5498f, 0.2873f);
    glVertex2f(-0.6218f, 0.3771f);
    glVertex2f(-0.6566f, 0.3778f);
    glVertex2f(-0.6489f, 0.2916f);
    glEnd();
    
    // Lower Wing
    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(-0.8513f, 0.1241f);
    glVertex2f(-0.8653f, 0.1246f);
    glVertex2f(-0.8522f, 0.1102f);
    glVertex2f(-0.8291f, 0.1111f);
    glVertex2f(-0.5056f, 0.2087f);
    glVertex2f(-0.7115f, 0.2078f);
    glEnd();
    
    // Lower Wing Back
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex2f(-0.8479f, 0.2812f);
    glVertex2f(-0.9293f, 0.2448f);
    glVertex2f(-0.9793f, 0.2453f);
    glVertex2f(-0.9320f, 0.2881f);
    glEnd();
    
    // Upper Wing Back
    glBegin(GL_POLYGON);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(-0.8229f, 0.3036f);
    glVertex2f(-0.8415f, 0.3121f);
    glVertex2f(-0.8562f, 0.3283f);
    glVertex2f(-0.9057f, 0.3910f);
    glVertex2f(-0.9428f, 0.3894f);
    
    
    glVertex2f(-0.9320f, 0.2881f);
    glVertex2f(-0.7618f, 0.2914f);
    glVertex2f(-0.7935f, 0.2959f);
    glVertex2f(-0.8097f, 0.2989f);
    glEnd();
    
    // ================= Back Door =================
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(-0.7514f, 0.2701f);
    glVertex2f(-0.7756f, 0.2687f);
    glVertex2f(-0.7743f, 0.2286f);
    glVertex2f(-0.7496f, 0.2281f);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex2f(-0.7514f, 0.2701f);
    glVertex2f(-0.7756f, 0.2687f);
    glVertex2f(-0.7743f, 0.2286f);
    glVertex2f(-0.7496f, 0.2281f);
    glEnd();
    
    // Back Door Handle
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(-0.7679f, 0.2511f);
    glVertex2f(-0.7709f, 0.2509f);
    glVertex2f(-0.7708f, 0.2474f);
    glVertex2f(-0.7677f, 0.2477f);
    glEnd();
    
    // ================= Windows =================
       glBegin(GL_QUADS);
       glColor3f(0.2f, 0.2f, 0.2f);

       // Window 1
       glVertex2f(-0.7151f, 0.2669f);
       glVertex2f(-0.7145f, 0.2435f);
       glVertex2f(-0.6963f, 0.2435f);
       glVertex2f(-0.6976f, 0.2669f);

       // Window 2
       glVertex2f(-0.6820f, 0.2682f);
       glVertex2f(-0.6813f, 0.2441f);
       glVertex2f(-0.6638f, 0.2445f);
       glVertex2f(-0.6641f, 0.2678f);

       // Window 3
       glVertex2f(-0.6501f, 0.2669f);
       glVertex2f(-0.6492f, 0.2448f);
       glVertex2f(-0.6316f, 0.2445f);
       glVertex2f(-0.6329f, 0.2672f);

       // Window 4
       glVertex2f(-0.6196f, 0.2672f);
       glVertex2f(-0.6183f, 0.2451f);
       glVertex2f(-0.6014f, 0.2448f);
       glVertex2f(-0.6030f, 0.2669f);

       // Window 5
       glVertex2f(-0.5858f, 0.2662f);
       glVertex2f(-0.5852f, 0.2458f);
       glVertex2f(-0.5702f, 0.2454f);
       glVertex2f(-0.5712f, 0.2669f);

       // Window 6
       glVertex2f(-0.5533f, 0.2669f);
       glVertex2f(-0.5520f, 0.2454f);
       glVertex2f(-0.5371f, 0.2448f);
       glVertex2f(-0.5394f, 0.2669f);

       // Window 7
       glVertex2f(-0.5228f, 0.2665f);
       glVertex2f(-0.5208f, 0.2432f);
       glVertex2f(-0.5040f, 0.2435f);
       glVertex2f(-0.5056f, 0.2669f);

       glEnd();

       glPopMatrix();
   }




// draws a ripple from x0…x1, interpolating base-height y0→y1
void drawRiverRipple(float x0, float x1, float y0, float y1) {
    glColor3ub(59, 116, 140);
    glLineWidth(2.0f);
    glBegin(GL_POLYGON);
    // sample resolution: increase for smoother wave
    const float step = 0.5f;
    for (float x = x0; x <= x1; x += step) {
        float t     = (x - x0) / (x1 - x0);
        float baseY = y0 + t * (y1 - y0);
        float y     = baseY + waveAmp * sinf(waveFreq * x + wavePhase);
        glVertex2f(x, y);
    }
    glEnd();
}





// Draw a filled circle at (cx,cy) with radius r
void drawCircle(float cx, float cy, float r, int segments = 32) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for(int i = 0; i <= segments; ++i) {
        float theta = 2.0f * PI * i / segments;
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta));
    }
    glEnd();
}

// Draws an improved stick-figure at (x,y), scaled by h
// – head circle, colored torso rectangle, thicker limbs
void drawHuman(float x, float y, float h = 1.0f) {
    // Head (skin tone)
    glColor3f(1.0f, 0.8f, 0.6f);
    drawCircle(x, y + 0.2f*h, 0.06f*h);

    // Torso (blue shirt)
    glColor3f(0.1f, 0.1f, 0.8f);
    glBegin(GL_POLYGON);
      glVertex2f(x - 0.03f*h, y + 0.15f*h);
      glVertex2f(x + 0.03f*h, y + 0.15f*h);
      glVertex2f(x + 0.05f*h, y - 0.1f*h);
      glVertex2f(x - 0.05f*h, y - 0.1f*h);
    glEnd();

    // Arms (blue)
    glLineWidth(5.0f);
    glColor3f(0.1f, 0.1f, 0.8f);
    glBegin(GL_LINES);
      // left arm
      glVertex2f(x,             y + 0.05f*h);
      glVertex2f(x - 0.15f*h,   y - 0.05f*h);
      // right arm
      glVertex2f(x,             y + 0.05f*h);
      glVertex2f(x + 0.15f*h,   y - 0.05f*h);
    glEnd();

    // Legs (dark gray pants)
    glLineWidth(6.0f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
      // left leg
      glVertex2f(x,             y - 0.1f*h);
      glVertex2f(x - 0.1f*h,    y - 0.3f*h);
      // right leg
      glVertex2f(x,             y - 0.1f*h);
      glVertex2f(x + 0.1f*h,    y - 0.3f*h);
    glEnd();
}

// Draws a diamond-kite centered at (kx,ky) of size s, string to (hx,hy)
void drawKite(float kx, float ky, float s, float hx, float hy) {
    // Kite body (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
      glVertex2f(kx        , ky      );
      glVertex2f(kx + s    , ky + s  );
      glVertex2f(kx        , ky + 2*s);
      glVertex2f(kx - s    , ky + s  );
    glEnd();

    // String (black wavy line)
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
      glVertex2f(kx            , ky           );
      glVertex2f(kx - 0.02f*s  , ky - 0.08f*s );
      glVertex2f(kx + 0.01f*s  , ky - 0.16f*s );
      glVertex2f(kx - 0.01f*s  , ky - 0.24f*s );
      glVertex2f(hx            , hy           );
    glEnd();
}


// Draw filled circle
void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.1416f * i / 100;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}




//draw line

// Draws a 3-blade grass tuft at (x, y)
void drawGrass(float x, float y,
               float bladeHeight = 1.0f,
               float spread = 0.5f,
               float lineWidth = 2.0f)
{
    glLineWidth(lineWidth);
    glColor3f(0.1f, 0.4f, 0.1f); // Grass green

    glBegin(GL_LINES);
        // Left blade
        glVertex2f(x, y);
        glVertex2f(x - spread, y + bladeHeight);
        // Center blade (taller)
        glVertex2f(x, y);
        glVertex2f(x, y + bladeHeight * 1.2f);
        // Right blade
        glVertex2f(x, y);
        glVertex2f(x + spread, y + bladeHeight);
    glEnd();
}

// Function to draw many blades to form a clump
void drawGrassClump(float x, float y, int count = 60)
{
    for (int i = 0; i < count; ++i)
    {
        float offsetX = (fakeRand(i * 2) - 0.5f) * 2.0f;         // -1.0 to +1.0
        float height  = 0.8f + fakeRand(i * 3) * 0.9f;           // 0.8 to 1.7
        float spread  = 0.3f + fakeRand(i * 4) * 0.4f;           // 0.3 to 0.7
        float thick   = 1.0f + fakeRand(i * 5) * 2.0f;           // 1.0 to 3.0

        drawGrass(x + offsetX, y, height, spread, thick);
    }
}

// draw a wheel centered at (0,0) of radius R with N buckets
void drawWheel(float R, int N) {
    // 1) RIM
    glColor3ub(139, 69, 19);
    glLineWidth(10.0f);
    glBegin(GL_LINE_LOOP);
      for(int i = 0; i < 100; ++i) {
          float theta = 2*PI * i / 100;
          glVertex2f(R * cos(theta), R * sin(theta));
      }
    glEnd();

    // 2) HUB
    glBegin(GL_POLYGON);
      for(int i = 0; i < 40; ++i) {
          float theta = 2*PI * i / 40;
          glVertex2f((R*0.2f) * cos(theta), (R*0.2f) * sin(theta));
      }
    glEnd();

    // 3) BUCKETS
    float bucketLen  = R * 0.2f;
    float halfAngWid = (2*PI / N) * 0.3f;
    glColor3ub(205, 133,  63);
    for(int i = 0; i < N; ++i) {
        float θ  = 2*PI * i / N;
        float a0 = θ - halfAngWid;
        float a1 = θ + halfAngWid;
        float r0 = R, r1 = R + bucketLen;

        float x0 = r0 * cos(a0), y0 = r0 * sin(a0);
        float x1 = r0 * cos(a1), y1 = r0 * sin(a1);
        float x2 = r1 * cos(a1), y2 = r1 * sin(a1);
        float x3 = r1 * cos(a0), y3 = r1 * sin(a0);

        glBegin(GL_QUADS);
          glVertex2f(x0, y0);
          glVertex2f(x1, y1);
          glVertex2f(x2, y2);
          glVertex2f(x3, y3);
        glEnd();
    }
}




// Draws a single 3‐blade grain stalk (“wheat”) at (x, y)
void drawGrain(float x, float y,
               float bladeHeight = 2.5f,
               float spread      = 0.3f,
               float lineWidth   = 0.1f)
{
    glLineWidth(lineWidth);
    // golden wheat color
    glColor3f(0.9f, 0.8f, 0.2f);

    glBegin(GL_LINES);
        // left awn
        glVertex2f(x, y);
        glVertex2f(x - spread, y + bladeHeight);
        // center awn (taller)
        glVertex2f(x, y);
        glVertex2f(x, y + bladeHeight * 1.3f);
        // right awn
        glVertex2f(x, y);
        glVertex2f(x + spread, y + bladeHeight);
    glEnd();
}

// Draws a rectangular patch of grain from (x0,y0) of size (width×height),
// arranged in a grid of rows×cols stalks.
inline void drawGrainField(float x0, float y0,
                           float width, float height,
                           int rows = 10, int cols = 25)
{
    float dx = width  / cols;
    float dy = height / rows;

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float xi = x0 + j * dx + dx * 0.5f;
            float yi = y0 + i * dy;
            float h = 1.2f + 0.3f * fakeRand(index * 3);
            float s = 0.25f + 0.1f * fakeRand(index * 5);
            drawGrain(xi, yi, h, s, 2.0f);
            ++index;
        }
    }
}


void ntree(){
    glBegin(GL_POLYGON);
    glColor3ub(139,69,19);
    glVertex2f(6.0f,42.0f);
    glVertex2f(8.0f,42.0f);
    glVertex2f(8.0f,50.0f);
    glVertex2f(6.0f,50.0f);
    glEnd();
    
    
    // Tree leaves (three green circles)
    
    
    
    
    drawCircle(7, 55, 4.5, 0.0f, 0.8f, 0.0f); // Center
    drawCircle(5, 52, 4.5, 0.0f, 0.7f, 0.0f); // Left
    drawCircle(10, 52, 4.5, 0.0f, 0.7f, 0.0f); // Right
    
    glBegin(GL_POLYGON);
    glColor3ub(0.0f, 0.7f, 0.0f);
    glVertex2f(6.0f,42.0f);
    glVertex2f(7.0f,42.0f);
    glVertex2f(5.0f,44.0f);
    
    glEnd();
    glBegin(GL_POLYGON);
    glColor3ub(0.0f, 0.7f, 0.0f);
    glVertex2f(6.0f,42.0f);
    glVertex2f(7.0f,42.0f);
    glVertex2f(7.5f,44.0f);
    
    glEnd();
    
    glBegin(GL_POLYGON);
    glColor3ub(0.0f, 0.7f, 0.0f);
    glVertex2f(7.0f,42.0f);
    glVertex2f(8.0f,42.0f);
    glVertex2f(9.0f,44.0f);
    
    glEnd();
}

void waterwheel(){
    glLineWidth(7.0f);

        // Set color (Red)
        glColor3f(0.65f, 0.16f, 0.16f);

        // Draw the wide line
        glBegin(GL_LINES);
            glVertex2f(65.0f, 24.0f);
            glVertex2f(65.0f, 20.0f);
        glEnd();
    
    const float centerX = 65.0f;
    const float centerY = 24.0f;
    const float radius  =  6.0f;
    const float axleLen = radius * 1.2f;  // 20% beyond rim

    // Position & rotate the wheel
    glPushMatrix();
      glTranslatef(centerX, centerY, 0.0f);
      glRotatef(wheelAngle, 0.0f, 0.0f, 1.0f);

      drawWheel(radius, 12);

      // Axle (rotates with wheel)
      glColor3ub(0,0,0);
      glLineWidth(3.0f);
      glBegin(GL_LINES);
        glVertex2f(-axleLen,  0.0f);
        glVertex2f( axleLen,  0.0f);
        glVertex2f(  0.0f, -axleLen);
        glVertex2f(  0.0f,  axleLen);
      glEnd();
    glPopMatrix();
}


void drawriverside(){
    glColor3ub(155, 188,  84);
    glBegin(GL_POLYGON);
      glVertex2f( 0.0f, 25.0f );
      glVertex2f(34.0f, 25.0f );
      glVertex2f(40.0f, 29.0f );
      glVertex2f(31.0f, 31.0f );
      glVertex2f(52.0f, 40.0f );
      glVertex2f(41.0f, 48.0f );
      glVertex2f(47.0f, 51.0f );
      glVertex2f(40.0f, 55.0f );
      glVertex2f( 0.0f, 55.0f );
    glEnd();

    // Right soil (green, via a fan)
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(155, 188,  84);        glVertex2f(73.0f, 44.0f);
          glVertex2f(58.0f, 24.0f);        // fan origin
          glVertex2f(67.0f, 16.0f);
          glVertex2f(100.0f,16.0f);
          glVertex2f(100.0f,55.0f);
          glVertex2f(64.0f, 55.0f);
          
        glEnd();

    // Brown “side‐face” under that slope (C→D→B→A)
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(42.3f, 47.3f);   // C
      glVertex2f(47.0f, 50.0f);   // D
      glVertex2f(47.0f, 51.0f);   // B
      glVertex2f(41.0f, 48.0f);   // A
    glEnd();
    
    glColor3ub(117,  86,   44);    glBegin(GL_TRIANGLE_FAN);
      glVertex2f(33.1f, 30.5f);   // C
      glVertex2f(52.0f, 38.0f);   // D
      glVertex2f(52.0f, 40.0f);   // B
      glVertex2f(31.0f, 31.0f);   // A
    glEnd();
    
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(35.0f, 24.0f);   // D
      glVertex2f(40.0f, 27.5f);
    glVertex2f(40.0f, 29.0f );// B
      glVertex2f(34.0f, 25.0f);   // A
    
    glEnd();
    
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(0.0f, 24.0f);   // D
      glVertex2f(35.0f, 24.0f);
    glVertex2f(34.0f, 25.0f );// B
      glVertex2f(0.0f, 25.0f);   // A
    
    glEnd();
    
    //------------------------------------
    //right
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(62.0f, 55.0f);   // D
      glVertex2f(72.0f, 42.5f);
    glVertex2f(73.0f, 44.0f );// B
      glVertex2f(64.0f, 55.0f);   // A
    
    glEnd();
    
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(58.0f, 24.0f);   // D
      glVertex2f(67.0f, 16.0f);
    glVertex2f(68.0f, 17.0f );// B
      glVertex2f(59.0f, 25.0f);   // A
    
    glEnd();
    
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(67.0f, 16.0f);   // D
      glVertex2f(100.0f, 16.0f);
    glVertex2f(100.0f, 17.0f );// B
      glVertex2f(68.0f, 17.0f);   // A
    
    glEnd();
    
}

void chaff(){
    
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(222,204,87);
    glVertex2f(24.0f,43.0f);
    glVertex2f(34.0f,43.0f);
    glVertex2f(34.0f,45.0f);
    glVertex2f(32.0f,47.0f);
    glVertex2f(28.0f,50.0f);
    glVertex2f(24.0f,47.0f);

    glEnd();
    
    
    glLineWidth(5.0f);

        
        glColor3f(0.0f, 0.0f, 0.0f);

        // Draw the wide line
        glBegin(GL_LINES);
            glVertex2f(28.0f, 50.0f);
            glVertex2f(28.0f, 51.0f);
        glEnd();
}

void house1d(){
    ///house on left side///
    //polygon 1
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(139,69,19);
        glVertex2f(11.0f,46.0f);
        glVertex2f(12.0f,46.0f);
        glVertex2f(14.0f,50.0f);
        

        glEnd();


    //polygon 2
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(139,69,19);
        glVertex2f(17.0f,46.0f);
        glVertex2f(25.0f,46.0f);
        glVertex2f(22.0f,50.0f);
        glVertex2f(14.0f,50.0f);

        glEnd();

    //polygon 3
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(205,133,63);
        glVertex2f(14.0f,50.0f);
        glVertex2f(12.0f,46.0f);
        glVertex2f(12.0f,43.0f);
        glVertex2f(17.0f,42.0f);
        glVertex2f(17.0f,46.0f);

        glEnd();

    //polygon 4
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(128,135,32);
        glVertex2f(17.0f,42.0f);
        glVertex2f(24.0f,43.0f);
        glVertex2f(24.0f,46.0f);
        glVertex2f(17.0f,46.0f);

        glEnd();

    //polygon 5
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(139,69,19);
        glVertex2f(11.5f,42.0f);
        glVertex2f(17.0f,41.0f);
        glVertex2f(17.0f,42.0f);
        glVertex2f(12.0f,43.0f);

        glEnd();

    //polygon 6
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(184,134,11);
        glVertex2f(17.0f,41.0f);
        glVertex2f(25.0f,42.0f);
        glVertex2f(24.0f,43.0f);
        glVertex2f(17.0f,42.0f);

        glEnd();

    //polygon 7
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(139,69,19);
    glVertex2f(14.0f,44.0f);
    glVertex2f(15.0f,44.2f);
    glVertex2f(15.0f,45.0f);
    glVertex2f(14.0f,45.0f);

        glEnd();


    ///door:left
    //polygon a
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(0,0,0);
    glVertex2f(14.0f,44.0f);
    glVertex2f(15.0f,44.2f);
    glVertex2f(15.0f,45.0f);
    glVertex2f(14.0f,45.0f);

    glEnd();

    //polygon b
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(139,69,19);
        glVertex2f(19.0f,42.2f);
        glVertex2f(20.5f,42.5f);
        glVertex2f(20.5f,45.0f);
        glVertex2f(19.0f,45.0f);

        glEnd();
}

void house3d(){
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(25.0f, 52.5f);
        glVertex2f(25.5f, 52.5f);
        glVertex2f(26.5f, 54.5f);
    glEnd();

    // Roof right
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(28.0f, 52.5f);
        glVertex2f(32.0f, 52.5f);
        glVertex2f(30.5f, 54.5f);
        glVertex2f(26.5f, 54.5f);
    glEnd();

    // Main wall
    glColor3ub(205, 133, 63);
    glBegin(GL_POLYGON);
        glVertex2f(26.5f, 54.5f);
        glVertex2f(25.5f, 52.5f);
        glVertex2f(25.5f, 51.0f);
        glVertex2f(28.0f, 50.5f);
        glVertex2f(28.0f, 52.5f);
    glEnd();

    // Front wall
    glColor3ub(128, 135, 32);
    glBegin(GL_POLYGON);
        glVertex2f(28.0f, 50.5f);
        glVertex2f(31.5f, 51.0f);
        glVertex2f(31.5f, 52.5f);
        glVertex2f(28.0f, 52.5f);
    glEnd();

    // Base trim left
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(25.25f, 50.5f);
        glVertex2f(28.0f, 50.0f);
        glVertex2f(28.0f, 50.5f);
        glVertex2f(25.5f, 51.0f);
    glEnd();

    // Base trim right
    glColor3ub(184, 134, 11);
    glBegin(GL_POLYGON);
        glVertex2f(28.0f, 50.0f);
        glVertex2f(32.0f, 50.5f);
        glVertex2f(31.5f, 51.0f);
        glVertex2f(28.0f, 50.5f);
    glEnd();

    // Small roof detail
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(26.5f, 51.5f);
        glVertex2f(27.0f, 51.6f);
        glVertex2f(27.0f, 52.0f);
        glVertex2f(26.5f, 52.0f);
    glEnd();

    // Door panel (black)
    glColor3ub(0, 0, 0);
    glBegin(GL_POLYGON);
        glVertex2f(26.5f, 51.5f);
        glVertex2f(27.0f, 51.6f);
        glVertex2f(27.0f, 52.0f);
        glVertex2f(26.5f, 52.0f);
    glEnd();

    // Door frame
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(29.0f, 50.6f);
        glVertex2f(29.75f, 50.75f);
        glVertex2f(29.75f, 52.0f);
        glVertex2f(29.0f, 52.0f);
    glEnd();
    
}
void house4d(){
    glColor3ub(105, 105, 105);  // dark gray
    glBegin(GL_POLYGON);
        glVertex2f(15.0f, 53.0f);
        glVertex2f(15.5f, 53.0f);
        glVertex2f(16.5f, 55.0f);
    glEnd();

    // Roof right
    glColor3ub(105, 105, 105);
    glBegin(GL_POLYGON);
        glVertex2f(18.0f, 53.0f);
        glVertex2f(22.0f, 53.0f);
        glVertex2f(20.5f, 55.0f);
        glVertex2f(16.5f, 55.0f);
    glEnd();

    // Main wall
    glColor3ub(173, 216, 230);  // light blue
    glBegin(GL_POLYGON);
        glVertex2f(16.5f, 55.0f);
        glVertex2f(15.5f, 53.0f);
        glVertex2f(15.5f, 51.5f);
        glVertex2f(18.0f, 51.0f);
        glVertex2f(18.0f, 53.0f);
    glEnd();

    // Front wall
    glColor3ub(107, 142,  35);  // olive green
    glBegin(GL_POLYGON);
        glVertex2f(18.0f, 51.0f);
        glVertex2f(21.5f, 51.5f);
        glVertex2f(21.5f, 53.0f);
        glVertex2f(18.0f, 53.0f);
    glEnd();

    // Base trim left
    glColor3ub(210, 105,  30);  // chocolate
    glBegin(GL_POLYGON);
        glVertex2f(15.25f, 51.0f);
        glVertex2f(18.0f, 50.5f);
        glVertex2f(18.0f, 51.0f);
        glVertex2f(15.5f, 51.5f);
    glEnd();

    // Base trim right
    glColor3ub(218, 165,  32);  // gold
    glBegin(GL_POLYGON);
        glVertex2f(18.0f, 50.5f);
        glVertex2f(22.0f, 51.0f);
        glVertex2f(21.5f, 51.5f);
        glVertex2f(18.0f, 51.0f);
    glEnd();

    // Small roof detail
    glColor3ub(105, 105, 105);
    glBegin(GL_POLYGON);
        glVertex2f(16.5f, 52.0f);
        glVertex2f(17.0f, 52.1f);
        glVertex2f(17.0f, 52.5f);
        glVertex2f(16.5f, 52.5f);
    glEnd();

    // Door panel (brown)
    glColor3ub(101,  67,  33);
    glBegin(GL_POLYGON);
        glVertex2f(16.5f, 52.0f);
        glVertex2f(17.0f, 52.1f);
        glVertex2f(17.0f, 52.5f);
        glVertex2f(16.5f, 52.5f);
    glEnd();

    // Door frame
    glColor3ub( 70,  70,  70);
    glBegin(GL_POLYGON);
        glVertex2f(19.0f, 51.1f);
        glVertex2f(19.75f, 51.25f);
        glVertex2f(19.75f, 52.5f);
        glVertex2f(19.0f, 52.5f);
    glEnd();
}

void house2(){
    //polygon 1
        glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
        glColor3ub(105, 105, 105);
        glVertex2f(17.0f,50.0f);
        glVertex2f(26.0f,50.0f);
        glVertex2f(24.0f,52.0f);
        glVertex2f(19.0f,52.0f);
        

        glEnd();
    
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(184,134,11);
    glVertex2f(24.0f,47.0f);
    glVertex2f(25.0f,48.0f);
    glVertex2f(25.0f,50.0f);
    glVertex2f(22.0f,50.0f);

    glEnd();
}

void house5d(){
    glPushMatrix();
    glColor3ub(169,  28,  10);
    glBegin(GL_POLYGON);
        glVertex2f(85.0f, 53.0f);
        glVertex2f(85.5f, 53.0f);
        glVertex2f(86.5f, 55.0f);
    glEnd();

    // Roof right
    glColor3ub(169,  28,  10);
    glBegin(GL_POLYGON);
        glVertex2f(89.0f, 53.0f);
        glVertex2f(93.0f, 53.0f);
        glVertex2f(91.5f, 55.0f);
        glVertex2f(86.5f, 55.0f);
    glEnd();

    // Main wall
    glColor3ub(245, 222, 179);
    glBegin(GL_POLYGON);
        glVertex2f(86.5f, 55.0f);
        glVertex2f(85.5f, 53.0f);
        glVertex2f(85.5f, 51.5f);
        glVertex2f(89.0f, 51.0f);
        glVertex2f(89.0f, 53.0f);
    glEnd();

    // Front wall
    glColor3ub(160,  82,  45);
    glBegin(GL_POLYGON);
        glVertex2f(89.0f, 51.0f);
        glVertex2f(92.5f, 51.5f);
        glVertex2f(92.5f, 53.0f);
        glVertex2f(89.0f, 53.0f);
    glEnd();

    // Base trim left
    glColor3ub(210, 105,  30);
    glBegin(GL_POLYGON);
        glVertex2f(85.25f, 51.0f);
        glVertex2f(89.0f, 50.5f);
        glVertex2f(89.0f, 51.0f);
        glVertex2f(85.5f, 51.5f);
    glEnd();

    // Base trim right
    glColor3ub(218, 165,  32);
    glBegin(GL_POLYGON);
        glVertex2f(89.0f, 50.5f);
        glVertex2f(93.0f, 51.0f);
        glVertex2f(92.5f, 51.5f);
        glVertex2f(89.0f, 51.0f);
    glEnd();

    // Small roof detail
    glColor3ub(169,  28,  10);
    glBegin(GL_POLYGON);
        glVertex2f(86.5f, 52.0f);
        glVertex2f(87.0f, 52.1f);
        glVertex2f(87.0f, 52.5f);
        glVertex2f(86.5f, 52.5f);
    glEnd();

    // Door panel
    glColor3ub(101,  67,  33);
    glBegin(GL_POLYGON);
        glVertex2f(86.5f, 52.0f);
        glVertex2f(87.0f, 52.1f);
        glVertex2f(87.0f, 52.5f);
        glVertex2f(86.5f, 52.5f);
    glEnd();

    // Door frame
    glColor3ub( 70,  70,  70);
    glBegin(GL_POLYGON);
        glVertex2f(89.0f, 51.1f);
        glVertex2f(89.75f, 51.25f);
        glVertex2f(89.75f, 52.5f);
        glVertex2f(89.0f, 52.5f);
    glEnd();
    glPopMatrix();
}

void house6d(){
    glPushMatrix();
    glTranslatef(-6.0f, 0.0f, 0.0f);  // shift everything left by 9

        // Roof left
        glColor3ub(169,  28,  10);
        glBegin(GL_POLYGON);
            glVertex2f(85.0f, 53.0f);
            glVertex2f(85.5f, 53.0f);
            glVertex2f(86.5f, 55.0f);
        glEnd();
        // Roof right
        glColor3ub(169,  28,  10);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 53.0f);
            glVertex2f(93.0f, 53.0f);
            glVertex2f(91.5f, 55.0f);
            glVertex2f(86.5f, 55.0f);
        glEnd();
        // Main wall
        glColor3ub(245, 222, 179);
        glBegin(GL_POLYGON);
            glVertex2f(86.5f, 55.0f);
            glVertex2f(85.5f, 53.0f);
            glVertex2f(85.5f, 51.5f);
            glVertex2f(89.0f, 51.0f);
            glVertex2f(89.0f, 53.0f);
        glEnd();
        // Front wall
        glColor3ub(160,  82,  45);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 51.0f);
            glVertex2f(92.5f, 51.5f);
            glVertex2f(92.5f, 53.0f);
            glVertex2f(89.0f, 53.0f);
        glEnd();
        // Base trim left
        glColor3ub(210, 105,  30);
        glBegin(GL_POLYGON);
            glVertex2f(85.25f, 51.0f);
            glVertex2f(89.0f, 50.5f);
            glVertex2f(89.0f, 51.0f);
            glVertex2f(85.5f, 51.5f);
        glEnd();
        // Base trim right
        glColor3ub(218, 165,  32);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 50.5f);
            glVertex2f(93.0f, 51.0f);
            glVertex2f(92.5f, 51.5f);
            glVertex2f(89.0f, 51.0f);
        glEnd();
        // Small roof detail
        glColor3ub(169,  28,  10);
        glBegin(GL_POLYGON);
            glVertex2f(86.5f, 52.0f);
            glVertex2f(87.0f, 52.1f);
            glVertex2f(87.0f, 52.5f);
            glVertex2f(86.5f, 52.5f);
        glEnd();
        // Door panel
        glColor3ub(101,  67,  33);
        glBegin(GL_POLYGON);
            glVertex2f(86.5f, 52.0f);
            glVertex2f(87.0f, 52.1f);
            glVertex2f(87.0f, 52.5f);
            glVertex2f(86.5f, 52.5f);
        glEnd();
        // Door frame
        glColor3ub( 70,  70,  70);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 51.1f);
            glVertex2f(89.75f, 51.25f);
            glVertex2f(89.75f, 52.5f);
            glVertex2f(89.0f, 52.5f);
        glEnd();

    glPopMatrix();

}

void fireN(){
    glPushMatrix();
    glTranslatef(35.0f, 45.0f, 0.0f);
    glScalef(2.5f, 2.5f, 1.0f);

    

    // Fireplace opening
    glColor3ub( 30,  30,  30);   // almost black
    glBegin(GL_POLYGON);
        glVertex2f(0.3f, 0.3f);
        glVertex2f(1.7f, 0.3f);
        glVertex2f(1.7f, 1.2f);
        glVertex2f(0.3f, 1.2f);
    glEnd();

    // Flame – yellow base
    glColor3ub(255, 255,   0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.5f, 1.2f);
        glVertex2f(1.0f, 1.6f);
        glVertex2f(1.5f, 1.2f);
    glEnd();

    // Flame – orange mid
    glColor3ub(255, 165,   0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.6f, 1.2f);
        glVertex2f(1.0f, 1.4f);
        glVertex2f(1.4f, 1.2f);
    glEnd();

    // Flame – red inner
    glColor3ub(255,   0,   0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.8f, 1.2f);
        glVertex2f(1.0f, 1.3f);
        glVertex2f(1.2f, 1.2f);
    glEnd();

    glPopMatrix();
}

void fireD(){
    glPushMatrix();
    glTranslatef(35.0f, 45.0f, 0.0f);
    glScalef(2.5f, 2.5f, 1.0f);

    

    // Fireplace opening
    glColor3ub( 30,  30,  30);   // almost black
    glBegin(GL_POLYGON);
        glVertex2f(0.3f, 0.3f);
        glVertex2f(1.7f, 0.3f);
        glVertex2f(1.7f, 1.2f);
        glVertex2f(0.3f, 1.2f);
    glEnd();
    glPopMatrix();
}

void hill(){
    //polygon a
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(47,74,46);
    glVertex2f(0.0f,55.0f);
    glVertex2f(55.0f,55.0f);
    glVertex2f(55.0f,56.0f);
    glVertex2f(45.0f,58.0f);
    glVertex2f(33.0f,60.0f);
    glVertex2f(22.0f,61.2f);
    glVertex2f(14.0f,62.0f);
    glVertex2f(8.0f,64.0f);
    glVertex2f(0.0f,65.0f);
    

    glEnd();
    
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(47,74,46);
    glVertex2f(55.0f,55.0f);
    glVertex2f(100.0f,55.0f);
    glVertex2f(100.0f,65.0f);
    glVertex2f(94.0f,64.5f);
    glVertex2f(85.0f,63.0f);
    glVertex2f(76.0f,60.0f);
    glVertex2f(70.0f,59.0f);
    glVertex2f(65.0f,57.0f);
    glVertex2f(55.0f,56.0f);
    
    glEnd();
}

void cloudsD(){
    glPushMatrix();
      glTranslatef(_moveC, 4, 0);
      drawCircle(10.0f, 65.0f, 2.0f,255,255,255);
      drawCircle(12.0f, 67.0f, 1.8f,255,255,255);
      drawCircle(14.0f, 65.0f, 2.0f,255,255,255);
      drawCircle(12.0f, 63.0f, 1.6f,255,255,255);
      drawCircle(16.0f, 65.0f, 1.8f,255,255,255);
      drawCircle(15.0f, 63.0f, 1.5f,255,255,255);
      drawCircle(15.0f, 67.0f, 1.5f,255,255,255);

    glPopMatrix();
    
    // --- Cloud 2: x≈45…51, y≈70 ---
    glColor3ub(255,255,255);        // white puffs
    glPushMatrix();
      glTranslatef(_moveC, 0.0f, 0.0f);
      drawCircle(45.0f, 70.0f, 2.5f,255,255,255);
      drawCircle(47.0f, 72.0f, 2.2f,255,255,255);
      drawCircle(49.0f, 70.0f, 2.5f,255,255,255);
      drawCircle(47.0f, 68.0f, 2.0f,255,255,255);
     // drawCircle(51.0f, 70.0f, 2.2f,255,255,255);
    glPopMatrix();
}

void cloud3D(){
    glColor3ub(255,255,255);
    glPushMatrix();
      glTranslatef(_moveC, 0.0f, 0.0f);
      drawCircle(75.0f, 68.0f, 2.3f,255,255,255);
      drawCircle(77.0f, 70.0f, 2.0f,255,255,255);
      drawCircle(79.0f, 68.0f, 2.3f,255,255,255);
      drawCircle(77.0f, 66.0f, 1.9f,255,255,255);
     // drawCircle(81.0f, 68.0f, 2.0f,255,255,255);
    glPopMatrix();
}


void house1N(){
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(11.0f,46.0f);
    glVertex2f(12.0f,46.0f);
    glVertex2f(14.0f,50.0f);
    glEnd();

//polygon 2
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(17.0f,46.0f);
    glVertex2f(25.0f,46.0f);
    glVertex2f(22.0f,50.0f);
    glVertex2f(14.0f,50.0f);

    glEnd();

//polygon 3
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(205,133,63);
    glVertex2f(14.0f,50.0f);
    glVertex2f(12.0f,46.0f);
    glVertex2f(12.0f,43.0f);
    glVertex2f(17.0f,42.0f);
    glVertex2f(17.0f,46.0f);

    glEnd();

//polygon 4
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(128,135,32);
    glVertex2f(17.0f,42.0f);
    glVertex2f(24.0f,43.0f);
    glVertex2f(24.0f,46.0f);
    glVertex2f(17.0f,46.0f);

    glEnd();

//polygon 5
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(11.5f,42.0f);
    glVertex2f(17.0f,41.0f);
    glVertex2f(17.0f,42.0f);
    glVertex2f(12.0f,43.0f);

    glEnd();

//polygon 6
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(184,134,11);
    glVertex2f(17.0f,41.0f);
    glVertex2f(25.0f,42.0f);
    glVertex2f(24.0f,43.0f);
    glVertex2f(17.0f,42.0f);

    glEnd();

//polygon 7
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
glVertex2f(14.0f,44.0f);
glVertex2f(15.0f,44.2f);
glVertex2f(15.0f,45.0f);
glVertex2f(14.0f,45.0f);

    glEnd();


///door:left
//polygon a
glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
glColor3ub(254,250,71);
glVertex2f(14.0f,44.0f);
glVertex2f(15.0f,44.2f);
glVertex2f(15.0f,45.0f);
glVertex2f(14.0f,45.0f);

glEnd();

//polygon b
    glBegin(GL_POLYGON);// Draw a Red 1x1 Square centered at origin
    glColor3ub(139,69,19);
    glVertex2f(19.0f,42.2f);
    glVertex2f(20.5f,42.5f);
    glVertex2f(20.5f,45.0f);
    glVertex2f(19.0f,45.0f);

    glEnd();
}


void house3N(){
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(25.0f, 52.5f);
        glVertex2f(25.5f, 52.5f);
        glVertex2f(26.5f, 54.5f);
    glEnd();

    // Roof right
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(28.0f, 52.5f);
        glVertex2f(32.0f, 52.5f);
        glVertex2f(30.5f, 54.5f);
        glVertex2f(26.5f, 54.5f);
    glEnd();

    // Main wall
    glColor3ub(205, 133, 63);
    glBegin(GL_POLYGON);
        glVertex2f(26.5f, 54.5f);
        glVertex2f(25.5f, 52.5f);
        glVertex2f(25.5f, 51.0f);
        glVertex2f(28.0f, 50.5f);
        glVertex2f(28.0f, 52.5f);
    glEnd();

    // Front wall
    glColor3ub(128, 135, 32);
    glBegin(GL_POLYGON);
        glVertex2f(28.0f, 50.5f);
        glVertex2f(31.5f, 51.0f);
        glVertex2f(31.5f, 52.5f);
        glVertex2f(28.0f, 52.5f);
    glEnd();

    // Base trim left
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(25.25f, 50.5f);
        glVertex2f(28.0f, 50.0f);
        glVertex2f(28.0f, 50.5f);
        glVertex2f(25.5f, 51.0f);
    glEnd();

    // Base trim right
    glColor3ub(184, 134, 11);
    glBegin(GL_POLYGON);
        glVertex2f(28.0f, 50.0f);
        glVertex2f(32.0f, 50.5f);
        glVertex2f(31.5f, 51.0f);
        glVertex2f(28.0f, 50.5f);
    glEnd();

    // Small roof detail
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(26.5f, 51.5f);
        glVertex2f(27.0f, 51.6f);
        glVertex2f(27.0f, 52.0f);
        glVertex2f(26.5f, 52.0f);
    glEnd();

    // Door panel (black)
    glColor3ub(254,250,71);
    glBegin(GL_POLYGON);
        glVertex2f(26.5f, 51.5f);
        glVertex2f(27.0f, 51.6f);
        glVertex2f(27.0f, 52.0f);
        glVertex2f(26.5f, 52.0f);
    glEnd();

    // Door frame
    glColor3ub(139, 69, 19);
    glBegin(GL_POLYGON);
        glVertex2f(29.0f, 50.6f);
        glVertex2f(29.75f, 50.75f);
        glVertex2f(29.75f, 52.0f);
        glVertex2f(29.0f, 52.0f);
    glEnd();
    
    
}

void house6N(){
    glPushMatrix();
    glTranslatef(-6.0f, 0.0f, 0.0f);  // shift everything left by 9

        // Roof left
        glColor3ub(169,  28,  10);
        glBegin(GL_POLYGON);
            glVertex2f(85.0f, 53.0f);
            glVertex2f(85.5f, 53.0f);
            glVertex2f(86.5f, 55.0f);
        glEnd();
        // Roof right
        glColor3ub(169,  28,  10);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 53.0f);
            glVertex2f(93.0f, 53.0f);
            glVertex2f(91.5f, 55.0f);
            glVertex2f(86.5f, 55.0f);
        glEnd();
        // Main wall
        glColor3ub(245, 222, 179);
        glBegin(GL_POLYGON);
            glVertex2f(86.5f, 55.0f);
            glVertex2f(85.5f, 53.0f);
            glVertex2f(85.5f, 51.5f);
            glVertex2f(89.0f, 51.0f);
            glVertex2f(89.0f, 53.0f);
        glEnd();
        // Front wall
        glColor3ub(160,  82,  45);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 51.0f);
            glVertex2f(92.5f, 51.5f);
            glVertex2f(92.5f, 53.0f);
            glVertex2f(89.0f, 53.0f);
        glEnd();
        // Base trim left
        glColor3ub(210, 105,  30);
        glBegin(GL_POLYGON);
            glVertex2f(85.25f, 51.0f);
            glVertex2f(89.0f, 50.5f);
            glVertex2f(89.0f, 51.0f);
            glVertex2f(85.5f, 51.5f);
        glEnd();
        // Base trim right
        glColor3ub(218, 165,  32);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 50.5f);
            glVertex2f(93.0f, 51.0f);
            glVertex2f(92.5f, 51.5f);
            glVertex2f(89.0f, 51.0f);
        glEnd();
        // Small roof detail
        glColor3ub(169,  28,  10);
        glBegin(GL_POLYGON);
            glVertex2f(86.5f, 52.0f);
            glVertex2f(87.0f, 52.1f);
            glVertex2f(87.0f, 52.5f);
            glVertex2f(86.5f, 52.5f);
        glEnd();
        // Door panel
        glColor3ub(101,  67,  33);
        glBegin(GL_POLYGON);
            glVertex2f(86.5f, 52.0f);
            glVertex2f(87.0f, 52.1f);
            glVertex2f(87.0f, 52.5f);
            glVertex2f(86.5f, 52.5f);
        glEnd();
        // Door frame
        glColor3ub( 254,250,71);
        glBegin(GL_POLYGON);
            glVertex2f(89.0f, 51.1f);
            glVertex2f(89.75f, 51.25f);
            glVertex2f(89.75f, 52.5f);
            glVertex2f(89.0f, 52.5f);
        glEnd();

    glPopMatrix();

}


void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    //sky
    glColor3ub(125, 201, 238);
       glBegin(GL_QUADS);
         glVertex2f( 0.0f, 55.0f );
         glVertex2f(100.0f, 55.0f );
         glVertex2f(100.0f, 80.0f );
         glVertex2f( 0.0f, 80.0f );
       glEnd();

       
       
       //sun -----------------
       
       drawCircle(65, 65, 3,255,255,0);
   

    
    drawriverside();
    
    
    // Left soil (green, arbitrary shape)
    
    
    
    
    //brus------------------
    chaff();
    
  //                     house 1
    house1d();
    
    
    //              house 3
    
    house3d();
    
    
    //              house 4
    
    house4d();
    
    
    //              house2.....
    
    house2();
    
    
    //right           house 5
    
    house5d();
   
    
    //                hoouse 6
    
    house6d();
  
    
    // ——— Fireplace ———---------------------------------------------
    fireD();
    

    
    // grain field .....................................
    
    
    
    
    
    drawGrainField(70.0f, 25.0f,
                   30.0f, 15.0f);
    
    


    //               hill 1............................-------------
    
    hill();
    
    
    
    
    //----------------------------------------------------------------------------------------
    
    ntree();
    //tree 2
    
    glPushMatrix(); // Save current matrix

        glTranslatef(73.0f, 32.0f, 0.0f); // Move the tree to new location
        glScalef(0.5f, 0.5f, 1.0f);
    
    ntree();

    glPopMatrix(); // Restore original matrix

    //tree 3
    
    glPushMatrix(); // Save current matrix

        glTranslatef(65.0f, 41.0f, 0.0f); // Move the tree to new location
        glScalef(0.3f, 0.3f, 1.0f);
    
    ntree();

    glPopMatrix();
    
    //tree4
    
    glPushMatrix(); // Save current matrix

        glTranslatef(93.0f, 30.0f, 0.0f); // Move the tree to new location
        glScalef(0.5f, 0.5f, 1.0f);
    
    ntree();
    glPopMatrix();
    
    //tree 5
    
    glPushMatrix(); // Save current matrix

        glTranslatef(40.0f, 30.0f, 0.0f); // Move the tree to new location
        glScalef(0.5f, 0.5f, 1.0f);
    
    ntree();
    glPopMatrix();
    
    //tree 6
    
    glPushMatrix(); // Save current matrix

        glTranslatef(33.0f, 41.0f, 0.0f); // Move the tree to new location
        glScalef(0.3f, 0.3f, 1.0f);
    
    ntree();
    glPopMatrix();
    
    
    

    //water-----------------------------------
    
    glBegin(GL_POLYGON);
    glColor3ub(81,144,168);
    glVertex2f(71.0f,23.0f);
    glVertex2f(100.0f,42.0f);
    glVertex2f(100.0f,44.0f);
    glVertex2f(71.0f,25.0f);
    
    
    glEnd();

    
    
    drawRiverRipple(0.0f, 62.0f,   // x0, x1
                    19.0f, 19.0f);
    
    
    
    //waterwheel --------------------------------------------------------
    
    waterwheel();
    
    //-----------water...
    
    glBegin(GL_POLYGON);
    glColor3ub(81,144,168);
    glVertex2f(62.0f,16.0f);
    glVertex2f(67.0f,16.0f);
    glVertex2f(62.0f,20.5f);
    
    glEnd();
    
    
    
    

    
    

    // Cloud 1 ,2
    cloudsD();
    
    
    //plane---------
    glPushMatrix();
    ndrawPlane();
    glPopMatrix();
    
    


    // --- Cloud 3: x≈75…81, y≈68 ---
   
    cloud3D();
    
   
    
    // Draw human at (0.2,0.2), scale 0.4
    float hx = 25.f, hy = 35.0f, h = 10.0f;
    drawHuman(hx, hy, h);
    
    glColor3ub(0,0,0);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
      glVertex2f(25.5f,  36.5f);
      glVertex2f (26.4f,  34.7f);
    glEnd();
    
    drawCircle(26.6f, 35.7f, 0.6,0,0,0);
    // Draw animated kite
    glPushMatrix();
    glTranslatef(kiteOffset, 0.0f, 0.0f);
    float kx = 40.0f + kiteOffset, ky = 60.0f, ks = 3.1f;
    drawKite(kx, ky, ks, hx + 0.1f*h, hy + 0.05f*h);
    glPopMatrix();
    
    
    
   


// wave---------------------------------------------
    
    drawRiverRipple(0.0f, 100.0f,   // x0, x1
                    13.0f, 13.0f);
    
    drawRiverRipple(0.0f, 100.0f,   // x0, x1
                    8.0f, 8.0f);
    
    drawRiverRipple(0.0f, 100.0f,   // x0, x1
                    3.0f, 3.0f);
    
    
    
    
    
    
    //grass...........
    
    drawGrassClump(5.0f, 25.0f, 60); drawGrassClump(2.0f, 25.0f, 60); drawGrassClump(8.0f, 25.0f, 60);
    drawGrassClump(11.0f, 25.0f, 60); drawGrassClump(14.0f, 25.0f, 60); drawGrassClump(17.0f, 25.0f, 60);
    drawGrassClump(20.0f, 25.0f, 60); drawGrassClump(23.0f, 25.0f, 60);   drawGrassClump(26.0f, 25.0f, 60);
    drawGrassClump(29.0f, 25.0f, 60);    drawGrassClump(32.0f, 25.0f, 60);    drawGrassClump(33.0f, 25.0f, 60);
    
    drawGrassClump(70.0f, 17.0f, 60);
    drawGrassClump(73.0f, 17.0f, 60);drawGrassClump(76.0f, 17.0f, 60);drawGrassClump(79.0f, 17.0f, 60);
    drawGrassClump(82.0f, 17.0f, 60);drawGrassClump(85.0f, 17.0f, 60);drawGrassClump(88.0f, 17.0f, 60);
    drawGrassClump(91.0f, 17.0f, 60);drawGrassClump(94.0f, 17.0f, 60);drawGrassClump(97.0f, 17.0f, 60);
    
    drawGrass(5,35); drawGrass(10,35); drawGrass(14,40); drawGrass(20,33); drawGrass(25,38); drawGrass(30,35); drawGrass(30,40);
    
    drawGrass(80,45); drawGrass(85,43); drawGrass(90,46); drawGrass(73,47);

    
    
    
    
    
    //boat-----
    
    //boaty1
    
    glPushMatrix();
        // horizontal animation
        glTranslatef(_moveA, 0.0f, 0.0f);

        // 1) move the original base-left (17,0) → (15,12)
        glTranslatef(75.0f, 10.0f, 0.0f);
        // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
        glScalef(2.6f, 2.6f, 1.0f);
        // 3) shift original coords so (17,0) becomes the new origin
        glTranslatef(-17.0f, 0.0f, 0.0f);
        nboat();
    glPopMatrix();
    
    
    // boat-2 motion right → left, scaled so its original base from x=[17→22], y=[0→...]
    
    
    glPushMatrix();
        // horizontal animation
        glTranslatef(_moveB, 0.0f, 0.0f);

        // 1) move the original base-left (17,0) → (15,12)
        glTranslatef(15.0f, 1.0f, 0.0f);
        // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
        glScalef(3.6f, 3.6f, 1.0f);
        // 3) shift original coords so (17,0) becomes the new origin
        glTranslatef(-17.0f, 0.0f, 0.0f);

    nboat();
    // 3) Red deck
    glBegin(GL_QUADS);
      glColor3ub(255, 99, 71);
      glVertex2f(20.5f,  1.5f);
      glVertex2f(21.0f,  3.5f);
      glVertex2f(18.5f,  3.5f);
      glVertex2f(18.0f,  1.5f);
    glEnd();

    // 4) Mast post
    glBegin(GL_QUADS);
      glColor3ub(139, 69, 19);
      glVertex2f(19.8f,  3.5f);
      glVertex2f(19.8f,  4.0f);
      glVertex2f(19.7f,  4.0f);
      glVertex2f(19.7f,  3.5f);
    glEnd();

    glPopMatrix();
    
    
    drawWindmill(72, 50);
    
   // drawRain();
    //drawSplashes();
    

    glutSwapBuffers();
}



void cloudsN(){
    // Cloud 1
    glPushMatrix();
      glTranslatef(_moveC, 4, 0);
      drawCircle(10.0f, 65.0f, 2.0f,128,178,177);
      drawCircle(12.0f, 67.0f, 1.8f,128,178,177);
      drawCircle(14.0f, 65.0f, 2.0f,128,178,177);
      drawCircle(12.0f, 63.0f, 1.6f,128,178,177);
      drawCircle(16.0f, 65.0f, 1.8f,128,178,177);
      drawCircle(15.0f, 63.0f, 1.5f,128,178,177);
      drawCircle(15.0f, 67.0f, 1.5f,128,178,177);

    glPopMatrix();
    
    // --- Cloud 2: x≈45…51, y≈70 ---
    glColor3ub(255,255,255);        // white puffs
    glPushMatrix();
      glTranslatef(_moveC, 0.0f, 0.0f);
      drawCircle(45.0f, 70.0f, 2.5f,128,178,177);
      drawCircle(47.0f, 72.0f, 2.2f,128,178,177);
      drawCircle(49.0f, 70.0f, 2.5f,128,178,177);
      drawCircle(47.0f, 68.0f, 2.0f,128,178,177);
     // drawCircle(51.0f, 70.0f, 2.2f,255,255,255);
    glPopMatrix();
}


void cloud3N(){
    glColor3ub(255,255,255);
    glPushMatrix();
      glTranslatef(_moveC, 0.0f, 0.0f);
      drawCircle(75.0f, 68.0f, 2.3f,128,178,177);
      drawCircle(77.0f, 70.0f, 2.0f,128,178,177);
      drawCircle(79.0f, 68.0f, 2.3f,128,178,177);
      drawCircle(77.0f, 66.0f, 1.9f,128,178,177);
     // drawCircle(81.0f, 68.0f, 2.0f,255,255,255);
    glPopMatrix();
}




void drawScene2() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Sky (blue band)
    // Night Sky
    glColor3ub(24, 84, 92);
    glBegin(GL_QUADS);
      glVertex2f(0.0f, 55.0f);
      glVertex2f(100.0f, 55.0f);
      glVertex2f(100.0f, 80.0f);
      glVertex2f(0.0f, 80.0f);
    glEnd();

    // Moon
    drawCircle(65, 65, 3, 0.996f, 0.980f, 0.714f);
    drawStars(500);

    
    
    
    drawriverside();
    
    // Left soil (green, arbitrary shape)
    
    
    
    
    //brus------------------
    chaff();
    
    
    ///house on left side///
    house1N();
       
    
    
    
    // house 3
    
    house3N();
    // --- Tiny House (scaled ~50%, around 25,50) ---
    // Roof left
   
    //house 4
    
    house4d();
    
    //house2.....
    house2();
    
    
    
    //right
    
    house5d();
    house6N();
   
    
    
    
    
  
    
    
    
    // ——— Fireplace ———---------------------------------------------
    fireN();
    
    
    // grain field .....................................
    
    
    
    drawGrainField(70.0f, 25.0f,
                   30.0f, 15.0f);
    
    
    //------------ grass field
    
     
    //hill 1............................-------------
    
    hill();
      
    
    //----------------------------------------------------------------------------------------
    
    ntree();
    //tree 2
    
    glPushMatrix(); // Save current matrix

        glTranslatef(73.0f, 32.0f, 0.0f); // Move the tree to new location
        glScalef(0.5f, 0.5f, 1.0f);
    
    ntree();

    glPopMatrix(); // Restore original matrix

    //tree 3
    
    glPushMatrix(); // Save current matrix

        glTranslatef(65.0f, 41.0f, 0.0f); // Move the tree to new location
        glScalef(0.3f, 0.3f, 1.0f);
    
    ntree();

    glPopMatrix();
    
    //tree4
    
    glPushMatrix(); // Save current matrix

        glTranslatef(93.0f, 30.0f, 0.0f); // Move the tree to new location
        glScalef(0.5f, 0.5f, 1.0f);
    
    ntree();
    glPopMatrix();
    
    //tree 5
    
    glPushMatrix(); // Save current matrix

        glTranslatef(40.0f, 30.0f, 0.0f); // Move the tree to new location
        glScalef(0.5f, 0.5f, 1.0f);
    
    ntree();
    glPopMatrix();
    
    //tree 6
    
    glPushMatrix(); // Save current matrix

        glTranslatef(33.0f, 41.0f, 0.0f); // Move the tree to new location
        glScalef(0.3f, 0.3f, 1.0f);
    
    ntree();
    glPopMatrix();
    
    
    

    //water-----------------------------------
    
    glBegin(GL_POLYGON);
    glColor3ub(81,144,168);
    glVertex2f(71.0f,23.0f);
    glVertex2f(100.0f,42.0f);
    glVertex2f(100.0f,44.0f);
    glVertex2f(71.0f,25.0f);
    
    
    glEnd();

    
    
    drawRiverRipple(0.0f, 62.0f,   // x0, x1
                    19.0f, 19.0f);
    
    
    
    //waterfan --------------------------------------------------------
    
    waterwheel();
    
    //-----------water...
    
    glBegin(GL_POLYGON);
    glColor3ub(81,144,168);
    glVertex2f(62.0f,16.0f);
    glVertex2f(67.0f,16.0f);
    glVertex2f(62.0f,20.5f);
    
    glEnd();
    
    
    
    

    
    

   
    //cloud1,2
    cloudsN();

    
    
    //plane---------
    glPushMatrix();
    ndrawPlane();
    glPopMatrix();
    
    


    // --- Cloud 3: x≈75…81, y≈68 ---
    cloud3N();
    
    
    
    
   


// wave---------------------------------------------
    
    drawRiverRipple(0.0f, 100.0f,   // x0, x1
                    13.0f, 13.0f);
    
    drawRiverRipple(0.0f, 100.0f,   // x0, x1
                    8.0f, 8.0f);
    
    drawRiverRipple(0.0f, 100.0f,   // x0, x1
                    3.0f, 3.0f);
    
    
    
    
    
    
    //grass...........
    
    drawGrassClump(5.0f, 25.0f, 60); drawGrassClump(2.0f, 25.0f, 60); drawGrassClump(8.0f, 25.0f, 60);
    drawGrassClump(11.0f, 25.0f, 60); drawGrassClump(14.0f, 25.0f, 60); drawGrassClump(17.0f, 25.0f, 60);
    drawGrassClump(20.0f, 25.0f, 60); drawGrassClump(23.0f, 25.0f, 60);   drawGrassClump(26.0f, 25.0f, 60);
    drawGrassClump(29.0f, 25.0f, 60);    drawGrassClump(32.0f, 25.0f, 60);    drawGrassClump(33.0f, 25.0f, 60);
    
    drawGrassClump(70.0f, 17.0f, 60);
    drawGrassClump(73.0f, 17.0f, 60);drawGrassClump(76.0f, 17.0f, 60);drawGrassClump(79.0f, 17.0f, 60);
    drawGrassClump(82.0f, 17.0f, 60);drawGrassClump(85.0f, 17.0f, 60);drawGrassClump(88.0f, 17.0f, 60);
    drawGrassClump(91.0f, 17.0f, 60);drawGrassClump(94.0f, 17.0f, 60);drawGrassClump(97.0f, 17.0f, 60);
    
    drawGrass(5,35); drawGrass(10,35); drawGrass(14,40); drawGrass(20,33); drawGrass(25,38); drawGrass(30,35); drawGrass(30,40);
    
    drawGrass(80,45); drawGrass(85,43); drawGrass(90,46); drawGrass(73,47);

    
    
    
    
    
    //boat-----
    
    glPushMatrix();
        // horizontal animation
        glTranslatef(_moveA, 0.0f, 0.0f);

        // 1) move the original base-left (17,0) → (15,12)
        glTranslatef(75.0f, 10.0f, 0.0f);
        // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
        glScalef(2.6f, 2.6f, 1.0f);
        // 3) shift original coords so (17,0) becomes the new origin
        glTranslatef(-17.0f, 0.0f, 0.0f);
        nboat();
    glPopMatrix();
    
    
    // boat-2 motion right → left, scaled so its original base from x=[17→22], y=[0→...]
    
    
    glPushMatrix();
        // horizontal animation
        glTranslatef(_moveB, 0.0f, 0.0f);

        // 1) move the original base-left (17,0) → (15,12)
        glTranslatef(15.0f, 1.0f, 0.0f);
        // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
        glScalef(3.6f, 3.6f, 1.0f);
        // 3) shift original coords so (17,0) becomes the new origin
        glTranslatef(-17.0f, 0.0f, 0.0f);

      nboat();
    
    glBegin(GL_QUADS);
      glColor3ub(255, 99, 71);
      glVertex2f(20.5f,  1.5f);
      glVertex2f(21.0f,  3.5f);
      glVertex2f(18.5f,  3.5f);
      glVertex2f(18.0f,  1.5f);
    glEnd();

    
    glBegin(GL_QUADS);
      glColor3ub(139, 69, 19);
      glVertex2f(19.8f,  3.5f);
      glVertex2f(19.8f,  4.0f);
      glVertex2f(19.7f,  4.0f);
      glVertex2f(19.7f,  3.5f);
    glEnd();

    glPopMatrix();
    
    drawWindmill(72, 50);
    
    //drawRain();
    //drawSplashes();
    
    //==============--------------------
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.3f); // Black overlay with 30% darkness
    glBegin(GL_QUADS);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(100.0f, 0.0f);
        glVertex2f(100.0f, 100.0f);
        glVertex2f(0.0f, 100.0f);
    glEnd();

    glDisable(GL_BLEND);
    

    glutSwapBuffers();
}

void riversideR(){
    glColor3ub(155, 188, 84);
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 25.0f);
    glVertex2f(34.0f, 25.0f);
    glVertex2f(40.0f, 29.0f);
    glVertex2f(31.0f, 31.0f);
    glVertex2f(52.0f, 40.0f);
    glVertex2f(41.0f, 48.0f);
    glVertex2f(47.0f, 51.0f);
    glVertex2f(40.0f, 55.0f);
    glVertex2f(0.0f, 55.0f);
    glEnd();

    // Right soil (green, via a fan)
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(155, 188, 84);        glVertex2f(73.0f, 44.0f);
    glVertex2f(58.0f, 24.0f);        // fan origin
    glVertex2f(67.0f, 16.0f);
    glVertex2f(100.0f, 16.0f);
    glVertex2f(100.0f, 55.0f);
    glVertex2f(64.0f, 55.0f);

    glEnd();

    // Brown “side‐face” under that slope (C→D→B→A)
    
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(58.0f, 24.0f);   // D
      glVertex2f(67.0f, 16.0f);
    glVertex2f(68.0f, 17.0f );// B
      glVertex2f(59.0f, 25.0f);   // A
    
    glEnd();
    
    glColor3ub(117,  86,   44);
    glBegin(GL_TRIANGLE_FAN);
        // C
      glVertex2f(67.0f, 16.0f);   // D
      glVertex2f(100.0f, 16.0f);
    glVertex2f(100.0f, 17.0f );// B
      glVertex2f(68.0f, 17.0f);   // A
    
    glEnd();
}

void cloudsDR(){
    // Cloud 1
    glPushMatrix();
    glTranslatef(_moveC, 4, 0);
    drawCircle(10.0f, 65.0f, 2.0f, 0.725f, 0.765f, 0.8f);
    drawCircle(12.0f, 67.0f, 1.8f, 0.725f, 0.765f, 0.8f);
    drawCircle(14.0f, 65.0f, 2.0f, 0.725f, 0.765f, 0.8f);
    drawCircle(12.0f, 63.0f, 1.6f, 0.725f, 0.765f, 0.8f);
    drawCircle(16.0f, 65.0f, 1.8f, 0.725f, 0.765f, 0.8f);
    drawCircle(15.0f, 63.0f, 1.5f, 0.725f, 0.765f, 0.8f);
    drawCircle(15.0f, 67.0f, 1.5f, 0.725f, 0.765f, 0.8f);

    glPopMatrix();

    // --- Cloud 2: x≈45…51, y≈70 ---
    glColor3ub(255, 255, 255);        // white puffs
    glPushMatrix();
    glTranslatef(_moveC, 0.0f, 0.0f);
    drawCircle(45.0f, 70.0f, 2.5f, 0.725f, 0.765f, 0.8f);
    drawCircle(47.0f, 72.0f, 2.2f, 0.725f, 0.765f, 0.8f);
    drawCircle(49.0f, 70.0f, 2.5f, 0.725f, 0.765f, 0.8f);
    drawCircle(47.0f, 68.0f, 2.0f, 0.725f, 0.765f, 0.8f);
    // drawCircle_N(51.0f, 70.0f, 2.2f,255,255,255);
    glPopMatrix();

}

void cloud3DR(){
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(_moveC, 0.0f, 0.0f);
    drawCircle(75.0f, 68.0f, 2.3f, 0.725f, 0.765f, 0.8f);
    drawCircle(77.0f, 70.0f, 2.0f, 0.725f, 0.765f, 0.8f);
    drawCircle(79.0f, 68.0f, 2.3f, 0.725f, 0.765f, 0.8f);
    drawCircle(77.0f, 66.0f, 1.9f, 0.725f, 0.765f, 0.8f);
    // drawCircle_N(81.0f, 68.0f, 2.0f,255,255,255);
    glPopMatrix();
}
void drawRainDayScene1_N() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glColor3ub(61, 100, 153);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 55.0f);
    glVertex2f(100.0f, 55.0f);
    glVertex2f(100.0f, 80.0f);
    glVertex2f(0.0f, 80.0f);
    glEnd();



    //sun -----------------





    // Left soil (green, arbitrary shape)
   
    riversideR();





    //brus------------------
    chaff();
    
  //                     house 1
    house1d();
    
    
    //              house 3
    
    house3d();
    
    
    //              house 4
    
    house4d();
    
    
    //              house2.....
    
    house2();
    
    
    //right           house 5
    
    house5d();
   
    
    //                hoouse 6
    
    house6d();
  
    
    // ——— Fireplace ———---------------------------------------------
    fireD();





    // grain field .....................................





    drawGrainField(70.0f, 25.0f,
        30.0f, 15.0f);


    //------------ grass field












    //hill 1............................-------------
    hill();

    



    //----------------------------------------------------------------------------------------

    ntree();
    //tree 2

    glPushMatrix(); // Save current matrix

    glTranslatef(73.0f, 32.0f, 0.0f); // Move the tree to new location
    glScalef(0.5f, 0.5f, 1.0f);

    ntree();

    glPopMatrix(); // Restore original matrix

    //tree 3

    glPushMatrix(); // Save current matrix

    glTranslatef(65.0f, 41.0f, 0.0f); // Move the tree to new location
    glScalef(0.3f, 0.3f, 1.0f);

    ntree();

    glPopMatrix();

    //tree4

    glPushMatrix(); // Save current matrix

    glTranslatef(93.0f, 30.0f, 0.0f); // Move the tree to new location
    glScalef(0.5f, 0.5f, 1.0f);

    ntree();
    glPopMatrix();

    //tree 5

    glPushMatrix(); // Save current matrix

    glTranslatef(40.0f, 30.0f, 0.0f); // Move the tree to new location
    glScalef(0.5f, 0.5f, 1.0f);

    ntree();
    glPopMatrix();

    //tree 6

    glPushMatrix(); // Save current matrix

    glTranslatef(33.0f, 41.0f, 0.0f); // Move the tree to new location
    glScalef(0.3f, 0.3f, 1.0f);

    ntree();
    glPopMatrix();




    //water-----------------------------------

    glBegin(GL_POLYGON);
    glColor3ub(81, 144, 168);
    glVertex2f(71.0f, 23.0f);
    glVertex2f(100.0f, 42.0f);
    glVertex2f(100.0f, 44.0f);
    glVertex2f(71.0f, 25.0f);


    glEnd();



    drawRiverRipple(0.0f, 62.0f,   // x0, x1
        19.0f, 19.0f);



    //waterwheel --------------------------------------------------------

    waterwheel();

    //-----------water...

    glBegin(GL_POLYGON);
    glColor3ub(81, 144, 168);
    glVertex2f(62.0f, 16.0f);
    glVertex2f(67.0f, 16.0f);
    glVertex2f(62.0f, 20.5f);

    glEnd();



// clouds.........
    cloudsDR();

    //plane---------
    glPushMatrix();
    ndrawPlane();
    glPopMatrix();




    // --- Cloud 3: x≈75…81, y≈68 ---
    cloud3DR();



    // wave---------------------------------------------

    drawRiverRipple(0.0f, 100.0f,   // x0, x1
        13.0f, 13.0f);

    drawRiverRipple(0.0f, 100.0f,   // x0, x1
        8.0f, 8.0f);

    drawRiverRipple(0.0f, 100.0f,   // x0, x1
        3.0f, 3.0f);






    //grass...........

    drawGrassClump(5.0f, 25.0f, 60); drawGrassClump(2.0f, 25.0f, 60); drawGrassClump(8.0f, 25.0f, 60);
    drawGrassClump(11.0f, 25.0f, 60); drawGrassClump(14.0f, 25.0f, 60); drawGrassClump(17.0f, 25.0f, 60);
    drawGrassClump(20.0f, 25.0f, 60); drawGrassClump(23.0f, 25.0f, 60);   drawGrassClump(26.0f, 25.0f, 60);
    drawGrassClump(29.0f, 25.0f, 60);    drawGrassClump(32.0f, 25.0f, 60);    drawGrassClump(33.0f, 25.0f, 60);

    drawGrassClump(70.0f, 17.0f, 60);
    drawGrassClump(73.0f, 17.0f, 60);drawGrassClump(76.0f, 17.0f, 60);drawGrassClump(79.0f, 17.0f, 60);
    drawGrassClump(82.0f, 17.0f, 60);drawGrassClump(85.0f, 17.0f, 60);drawGrassClump(88.0f, 17.0f, 60);
    drawGrassClump(91.0f, 17.0f, 60);drawGrassClump(94.0f, 17.0f, 60);drawGrassClump(97.0f, 17.0f, 60);

    drawGrass(5, 35); drawGrass(10, 35); drawGrass(14, 40); drawGrass(20, 33); drawGrass(25, 38); drawGrass(30, 35); drawGrass(30, 40);

    drawGrass(80, 45); drawGrass(85, 43); drawGrass(90, 46); drawGrass(73, 47);






    //boat-----

    //boaty1

    glPushMatrix();
    // horizontal animation
    glTranslatef(_moveA, 0.0f, 0.0f);

    // 1) move the original base-left (17,0) → (15,12)
    glTranslatef(75.0f, 10.0f, 0.0f);
    // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
    glScalef(2.6f, 2.6f, 1.0f);
    // 3) shift original coords so (17,0) becomes the new origin
    glTranslatef(-17.0f, 0.0f, 0.0f);
    nboat();
    glPopMatrix();


    // boat-2 motion right → left, scaled so its original base from x=[17→22], y=[0→...]


    glPushMatrix();
    // horizontal animation
    glTranslatef(_moveB, 0.0f, 0.0f);

    // 1) move the original base-left (17,0) → (15,12)
    glTranslatef(15.0f, 1.0f, 0.0f);
    // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
    glScalef(3.6f, 3.6f, 1.0f);
    // 3) shift original coords so (17,0) becomes the new origin
    glTranslatef(-17.0f, 0.0f, 0.0f);

    nboat();
    // 3) Red deck
    glBegin(GL_QUADS);
    glColor3ub(255, 99, 71);
    glVertex2f(20.5f, 1.5f);
    glVertex2f(21.0f, 3.5f);
    glVertex2f(18.5f, 3.5f);
    glVertex2f(18.0f, 1.5f);
    glEnd();

    // 4) Mast post
    glBegin(GL_QUADS);
    glColor3ub(139, 69, 19);
    glVertex2f(19.8f, 3.5f);
    glVertex2f(19.8f, 4.0f);
    glVertex2f(19.7f, 4.0f);
    glVertex2f(19.7f, 3.5f);
    glEnd();

    glPopMatrix();


    drawWindmill(72, 50);

    drawRain();
    drawSplashes();


    glutSwapBuffers();
}



void cloudsNR(){
    glPushMatrix();
    glTranslatef(_moveC, 4, 0);
    drawCircle(10.0f, 65.0f, 2.0f, 0.157f, 0.275f, 0.369f);
    drawCircle(12.0f, 67.0f, 1.8f, 0.157f, 0.275f, 0.369f);
    drawCircle(14.0f, 65.0f, 2.0f, 0.157f, 0.275f, 0.369f);
    drawCircle(12.0f, 63.0f, 1.6f, 0.157f, 0.275f, 0.369f);
    drawCircle(16.0f, 65.0f, 1.8f, 0.157f, 0.275f, 0.369f);
    drawCircle(15.0f, 63.0f, 1.5f, 0.157f, 0.275f, 0.369f);
    drawCircle(15.0f, 67.0f, 1.5f, 0.157f, 0.275f, 0.369f);

    glPopMatrix();

    // --- Cloud 2: x≈45…51, y≈70 ---
    glColor3ub(255, 255, 255);        // white puffs
    glPushMatrix();
    glTranslatef(_moveC, 0.0f, 0.0f);
    drawCircle(45.0f, 70.0f, 2.5f,0.157f, 0.275f, 0.369f);
    drawCircle(47.0f, 72.0f, 2.2f, 0.157f, 0.275f, 0.369f);
    drawCircle(49.0f, 70.0f, 2.5f, 0.157f, 0.275f, 0.369f);
    drawCircle(47.0f, 68.0f, 2.0f, 0.157f, 0.275f, 0.369f);
    // drawCircle_N(51.0f, 70.0f, 2.2f,255,255,255);
    glPopMatrix();
}

void cloud3NR(){
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslatef(_moveC, 0.0f, 0.0f);
    drawCircle(75.0f, 68.0f, 2.3f, 0.157f, 0.275f, 0.369f);
    drawCircle(77.0f, 70.0f, 2.0f,0.157f, 0.275f, 0.369f);
    drawCircle(79.0f, 68.0f, 2.3f, 0.157f, 0.275f, 0.369f);
    drawCircle(77.0f, 66.0f, 1.9f, 0.157f, 0.275f, 0.369f);
    // drawCircle_N(81.0f, 68.0f, 2.0f,255,255,255);
    glPopMatrix();
}


void drawRainNightScene1_N() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Sky (blue band)
    // Night Sky
    glColor3ub(25, 43, 60);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 55.0f);
    glVertex2f(100.0f, 55.0f);
    glVertex2f(100.0f, 80.0f);
    glVertex2f(0.0f, 80.0f);
    glEnd();

    




    // Left soil (green, arbitrary shape)
    riversideR();
 //brus------------------

    //brus------------------
    chaff();
    
    
    ///house on left side///
    house1N();
       
    
    
    
    // house 3
    
    house3N();
    // --- Tiny House (scaled ~50%, around 25,50) ---
    // Roof left
   
    //house 4
    
    house4d();
    
    //house2.....
    house2();
    
    
    
    //right
    
    house5d();
    house6N();
   
    
    
    
    
  
    
    
    
    // ——— Fireplace ———---------------------------------------------
    fireD();





    // grain field .....................................





    drawGrainField(70.0f, 25.0f,
        30.0f, 15.0f);


    //------------ grass field












    //hill 1............................-------------

    hill();
    

    //----------------------------------------------------------------------------------------

    ntree();
    //tree 2

    glPushMatrix(); // Save current matrix

    glTranslatef(73.0f, 32.0f, 0.0f); // Move the tree to new location
    glScalef(0.5f, 0.5f, 1.0f);

    ntree();

    glPopMatrix(); // Restore original matrix

    //tree 3

    glPushMatrix(); // Save current matrix

    glTranslatef(65.0f, 41.0f, 0.0f); // Move the tree to new location
    glScalef(0.3f, 0.3f, 1.0f);

    ntree();

    glPopMatrix();

    //tree4

    glPushMatrix(); // Save current matrix

    glTranslatef(93.0f, 30.0f, 0.0f); // Move the tree to new location
    glScalef(0.5f, 0.5f, 1.0f);

    ntree();
    glPopMatrix();

    //tree 5

    glPushMatrix(); // Save current matrix

    glTranslatef(40.0f, 30.0f, 0.0f); // Move the tree to new location
    glScalef(0.5f, 0.5f, 1.0f);

    ntree();
    glPopMatrix();

    //tree 6

    glPushMatrix(); // Save current matrix

    glTranslatef(33.0f, 41.0f, 0.0f); // Move the tree to new location
    glScalef(0.3f, 0.3f, 1.0f);

    ntree();
    glPopMatrix();




    //water-----------------------------------

    glBegin(GL_POLYGON);
    glColor3ub(81, 144, 168);
    glVertex2f(71.0f, 23.0f);
    glVertex2f(100.0f, 42.0f);
    glVertex2f(100.0f, 44.0f);
    glVertex2f(71.0f, 25.0f);


    glEnd();



    drawRiverRipple(0.0f, 62.0f,   // x0, x1
        19.0f, 19.0f);



    //waterfan --------------------------------------------------------

    waterwheel();

    //-----------water...

    glBegin(GL_POLYGON);
    glColor3ub(81, 144, 168);
    glVertex2f(62.0f, 16.0f);
    glVertex2f(67.0f, 16.0f);
    glVertex2f(62.0f, 20.5f);

    glEnd();








    // Cloud 1
    cloudsNR();




    //plane---------
    glPushMatrix();
    ndrawPlane();
    glPopMatrix();




    // --- Cloud 3: x≈75…81, y≈68 ---
    cloud3NR();








    // wave---------------------------------------------

    drawRiverRipple(0.0f, 100.0f,   // x0, x1
        13.0f, 13.0f);

    drawRiverRipple(0.0f, 100.0f,   // x0, x1
        8.0f, 8.0f);

    drawRiverRipple(0.0f, 100.0f,   // x0, x1
        3.0f, 3.0f);






    //grass...........

    drawGrassClump(5.0f, 25.0f, 60); drawGrassClump(2.0f, 25.0f, 60); drawGrassClump(8.0f, 25.0f, 60);
    drawGrassClump(11.0f, 25.0f, 60); drawGrassClump(14.0f, 25.0f, 60); drawGrassClump(17.0f, 25.0f, 60);
    drawGrassClump(20.0f, 25.0f, 60); drawGrassClump(23.0f, 25.0f, 60);   drawGrassClump(26.0f, 25.0f, 60);
    drawGrassClump(29.0f, 25.0f, 60);    drawGrassClump(32.0f, 25.0f, 60);    drawGrassClump(33.0f, 25.0f, 60);

    drawGrassClump(70.0f, 17.0f, 60);
    drawGrassClump(73.0f, 17.0f, 60);drawGrassClump(76.0f, 17.0f, 60);drawGrassClump(79.0f, 17.0f, 60);
    drawGrassClump(82.0f, 17.0f, 60);drawGrassClump(85.0f, 17.0f, 60);drawGrassClump(88.0f, 17.0f, 60);
    drawGrassClump(91.0f, 17.0f, 60);drawGrassClump(94.0f, 17.0f, 60);drawGrassClump(97.0f, 17.0f, 60);

    drawGrass(5, 35); drawGrass(10, 35); drawGrass(14, 40); drawGrass(20, 33); drawGrass(25, 38); drawGrass(30, 35); drawGrass(30, 40);

    drawGrass(80, 45); drawGrass(85, 43); drawGrass(90, 46); drawGrass(73, 47);






    //boat-----

    glPushMatrix();
    // horizontal animation
    glTranslatef(_moveA, 0.0f, 0.0f);

    // 1) move the original base-left (17,0) → (15,12)
    glTranslatef(75.0f, 10.0f, 0.0f);
    // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
    glScalef(2.6f, 2.6f, 1.0f);
    // 3) shift original coords so (17,0) becomes the new origin
    glTranslatef(-17.0f, 0.0f, 0.0f);
    nboat();
    glPopMatrix();


    // boat-2 motion right → left, scaled so its original base from x=[17→22], y=[0→...]


    glPushMatrix();
    // horizontal animation
    glTranslatef(_moveB, 0.0f, 0.0f);

    // 1) move the original base-left (17,0) → (15,12)
    glTranslatef(15.0f, 1.0f, 0.0f);
    // 2) scale so width 22−17 = 5 → 33−15 = 18  ⇒ scale = 18/5 = 3.6
    glScalef(3.6f, 3.6f, 1.0f);
    // 3) shift original coords so (17,0) becomes the new origin
    glTranslatef(-17.0f, 0.0f, 0.0f);

    nboat();

    glBegin(GL_QUADS);
    glColor3ub(255, 99, 71);
    glVertex2f(20.5f, 1.5f);
    glVertex2f(21.0f, 3.5f);
    glVertex2f(18.5f, 3.5f);
    glVertex2f(18.0f, 1.5f);
    glEnd();


    glBegin(GL_QUADS);
    glColor3ub(139, 69, 19);
    glVertex2f(19.8f, 3.5f);
    glVertex2f(19.8f, 4.0f);
    glVertex2f(19.7f, 4.0f);
    glVertex2f(19.7f, 3.5f);
    glEnd();

    glPopMatrix();

    drawWindmill(72, 50);

    //==============--------------------

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.3f); // Black overlay with 30% darkness
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(100.0f, 0.0f);
    glVertex2f(100.0f, 100.0f);
    glVertex2f(0.0f, 100.0f);
    glEnd();

    glDisable(GL_BLEND);


    drawRain();
    drawSplashes();

    glutSwapBuffers();
}




void updateRipple() {
    wavePhase += waveSpeed;
    if (wavePhase > 2*PI) wavePhase -= 2*PI;
    glutPostRedisplay();
}

void update4(int value) {
    
    _moveA += speed;
    if(_moveA > 90)
    {
        _moveA = -90;
    }
    
    if (_moveB > +95) {
        _moveB = -90;
    }
    _moveB += 0.15;
    if (_moveC > +95) {
        _moveC = -90;
    }
    _moveC += 0.04;

    // --- Kite movement logic ---
    kiteOffset += kiteDir * 0.007f;
    if (kiteOffset > 1.0f || kiteOffset < -1.0f) kiteDir = -kiteDir;

    // --- Ripple effect update ---
    updateRipple();

    // --- Wheel rotation logic ---
    if (rotating) {
        wheelAngle -= speed1;
        if (wheelAngle <= targetAngle) {
            wheelAngle = targetAngle;
            rotating = false;
        }
    }
    
    angle-=2.0f;
    if(angle > 360.0)
    {
        angle-=360;
    }
    
    planeX -= planeSpeed;
        if (planeX < -30.0f) planeX = 120.0f;

    // --- Redraw the scene ---
    glutPostRedisplay();

    // --- Set timer again ---
    glutTimerFunc(20, update4, 0);
}
void updateRain(int value) {
    for (int i = 0; i < MAX_RAINDROPS; i++) {
        rain[i].y -= rainSpeed;

        // Check if it has reached the target splash point
        if (rain[i].y <= rain[i].targetY) {
            addSplash(rain[i].x, rain[i].targetY);

            // Reset raindrop
            rain[i].x = static_cast<float>(rand()) / RAND_MAX * 100.0f;
            rain[i].y = 100 + rand() % 50;
            rain[i].targetY = rand() % 51; // New target Y between 0 and 60
        }
    }

    // Update splashes
    for (int i = 0; i < MAX_SPLASHES; i++) {
        if (splashes[i].active) {
            splashes[i].radius += 0.05f;
            splashes[i].alpha -= 0.03f;
            if (splashes[i].alpha <= 0.0f) {
                splashes[i].active = 0;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateRain, 0);
}


void handleMouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON)
    {
        speed += 0.05f;
        planeSpeed += 0.05f;
       
    }


    else if (button == GLUT_RIGHT_BUTTON)
    {
        speed -= 0.05f;

        if (planeSpeed > 0.4) {
            planeSpeed -= 0.05f;
        }

        _moveC -= 0.05f;
    }
glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y) {
    if((key=='w'||key=='W') && !rotating) {
        // start one clockwise revolution
        targetAngle = wheelAngle - 360.0f;
        rotating    = true;
    }
    switch (key) {
        case 's'://stops
            speed = 0.0f;
            break;
        case 'r'://runs
            speed = 0.02f;
            planeSpeed=0.2f*2;
            rainSpeed=0.6;
            break;
    }
    switch(key)
    {
    case '1':

     glutDisplayFunc(drawScene);
    break;
    case '3':

     glutDisplayFunc(drawScene2);
    break;
        case '2':
            glutDisplayFunc(drawRainDayScene1_N);
            break;
        case '4':
            glutDisplayFunc(drawRainNightScene1_N);
            break;
        case '+':
             rainSpeed += 0.1f;
             if (rainSpeed > 5.0f) rainSpeed = 5.0f;
             break;

         case '-':
             rainSpeed -= 0.1f;
             if (rainSpeed < 0.1f) rainSpeed = 0.1f;
             break;

    }
    glutPostRedisplay();
}








int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1500, 1000);
    glutCreateWindow("Village");
    

    init();
    glutDisplayFunc(drawScene);
    glutTimerFunc(20, update4, 0);
    glutTimerFunc(0, updateRain, 0);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(handleMouse);
    glutMainLoop();
    return 0;
}
