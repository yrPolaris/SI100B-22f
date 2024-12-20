#include "lcd/lcd.h"
#include "utils.h"
#include "assembly/example.h"

#define ENTITY_SIZE 2
#define WALL_WIDTH 1
#define EASY_WALL_DIS 60
#define HARD_WALL_DIS 45
#define EASY_WALL_GAP 60
#define HARD_WALL_GAP 30
#define EASY_g 1
#define HARD_g 2
#define EASY_upv -5
#define HARD_upv -7
#define FRAME_TIME 20

struct Entity
{
  u16 x;
  u16 y;
  u16 v;
  u16 color;
};

struct Tail_dot
{
  u16 x;
  u16 y;
  u16 color;
};

struct Wall
{
  u16 x;
  u16 y_gap;
};

void DrawEntity(struct Entity *bird, u16 color)
{
  LCD_Fill(bird->x, bird->y, bird->x + ENTITY_SIZE, bird->y + ENTITY_SIZE, color);
}

void DrawWall(struct Wall *wall, int difficulty, u16 color)
{
  if (difficulty == 0)
  {
    LCD_Fill(wall->x - WALL_WIDTH, 40, wall->x, wall->y_gap, color);
    LCD_Fill(wall->x - WALL_WIDTH, wall->y_gap + EASY_WALL_GAP, wall->x, LCD_H, color);
  }
  else
  {
    LCD_Fill(wall->x - WALL_WIDTH, 40, wall->x, wall->y_gap, color);
    LCD_Fill(wall->x - WALL_WIDTH, wall->y_gap + HARD_WALL_GAP, wall->x, LCD_H, color);
  }
}

void DrawTail(struct Tail_dot *behind_tail, struct Tail_dot *front_tail, struct Entity *bird, u16 color)
{
  LCD_DrawPoint(behind_tail->x, behind_tail->y, color);
  LCD_DrawLine(behind_tail->x, behind_tail->y, front_tail->x, front_tail->y, color);
}

void UpdateEntity(struct Entity *bird, int birdisup, int difficulty)
{
  DrawEntity(bird, BLACK);
  if (birdisup)
  {
    bird->y += bird->v;
  }
  else
  {
    if (bird->v < 0)
    {
      bird->v = 0;
    }
    if (difficulty == 0)
    {
      bird->v += EASY_g;
      bird->y += bird->v;
    }
    else
    {
      bird->v += HARD_g;
      bird->y += bird->v;
    }
  }
  DrawEntity(bird, bird->color);
}

void UpdateWall(struct Wall *wall, int difficulty)
{
  if (difficulty == 0)
  {
    if (wall[0].x > LCD_W - EASY_WALL_DIS)
    {
      DrawWall(&wall[0], difficulty, BLACK);
      wall[0].x -= 1;
      DrawWall(&wall[0], difficulty, WHITE);
    }
    else if (wall[0].x <= LCD_W - EASY_WALL_DIS && wall[0].x >= WALL_WIDTH + 1)
    {
      DrawWall(&wall[0], difficulty, BLACK);
      wall[0].x -= 1;
      DrawWall(&wall[0], difficulty, WHITE);
      DrawWall(&wall[1], difficulty, BLACK);
      wall[1].x -= 1;
      DrawWall(&wall[1], difficulty, WHITE);
    }
    else if (wall[0].x <= WALL_WIDTH)
    {
      DrawWall(&wall[0], difficulty, BLACK);
      DrawWall(&wall[1], difficulty, BLACK);
      wall[0].x = wall[1].x;
      wall[0].y_gap = wall[1].y_gap;
      wall[0].x -= 1;
      wall[1].x = LCD_W;
      wall[1].y_gap = rand() % (LCD_H - 40 - EASY_WALL_GAP) + 40;
      DrawWall(&wall[0], difficulty, WHITE);
    }
  }
  else
  {
    if (wall[0].x > LCD_W - HARD_WALL_DIS)
    {
      DrawWall(&wall[0], difficulty, BLACK);
      wall[0].x -= 1;
      DrawWall(&wall[0], difficulty, WHITE);
    }
    else if (wall[0].x <= LCD_W - HARD_WALL_DIS && wall[0].x >= WALL_WIDTH + 1)
    {
      DrawWall(&wall[0], difficulty, BLACK);
      wall[0].x -= 1;
      DrawWall(&wall[0], difficulty, WHITE);
      DrawWall(&wall[1], difficulty, BLACK);
      wall[1].x -= 1;
      DrawWall(&wall[1], difficulty, WHITE);
    }
    else if (wall[0].x <= WALL_WIDTH)
    {
      DrawWall(&wall[0], difficulty, BLACK);
      DrawWall(&wall[1], difficulty, BLACK);
      wall[0].x = wall[1].x;
      wall[0].y_gap = wall[1].y_gap;
      wall[0].x -= 1;
      wall[1].x = LCD_W;
      wall[1].y_gap = rand() % (LCD_H - 40 - HARD_WALL_GAP) + 40;
      DrawWall(&wall[0], difficulty, WHITE);
    }
  }
}

void UpdateTail(struct Tail_dot *tail, struct Entity *bird)
{
  for (int i = 14; i >= 1; i--)
  {
    DrawTail(&tail[i], &tail[i - 1], bird, BLACK);
  }
  for (int i = 14; i > 0; i--)
  {
    tail[i].color = tail[i - 1].color;
    tail[i].y = tail[i - 1].y;
  }
  tail[0].color = bird->color;
  tail[0].y = bird->y;
  for (int i = 14; i >= 1; i--)
  {
    DrawTail(&tail[i], &tail[i - 1], bird, tail[i].color);
  }
}

int check_collision(struct Entity *bird, struct Wall *wall, int difficulty)
{
  if (difficulty == 0)
  {
    if (bird->x + ENTITY_SIZE > wall[0].x - WALL_WIDTH && bird->x < wall[0].x)
    {
      if (bird->y < wall[0].y_gap || bird->y + ENTITY_SIZE > wall[0].y_gap + EASY_WALL_GAP)
      {
        return 1;
      }
    }
  }
  else
  {
    if (bird->x + ENTITY_SIZE > wall[0].x - WALL_WIDTH && bird->x < wall[0].x)
    {
      if (bird->y < wall[0].y_gap || bird->y + ENTITY_SIZE > wall[0].y_gap + HARD_WALL_GAP)
      {
        return 1;
      }
    }
  }
  if (difficulty == 0)
  {
    if (bird->x + ENTITY_SIZE > wall[1].x - WALL_WIDTH && bird->x < wall[1].x)
    {
      if (bird->y < wall[1].y_gap || bird->y + ENTITY_SIZE > wall[1].y_gap + EASY_WALL_GAP)
      {
        return 1;
      }
    }
  }
  else
  {
    if (bird->x + ENTITY_SIZE > wall[1].x - WALL_WIDTH && bird->x < wall[1].x)
    {
      if (bird->y < wall[1].y_gap || bird->y + ENTITY_SIZE > wall[1].y_gap + HARD_WALL_GAP)
      {
        return 1;
      }
    }
  }
  if (bird->y < ENTITY_SIZE - 1 + 40 || bird->y >= LCD_H - ENTITY_SIZE + 1)
  {
    return 1;
  }
  return 0;
}

void Inp_init(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOC);

  gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
}

void IO_init(void)
{
  Inp_init(); // inport init
  Lcd_Init(); // LCD init
}

int main(void)
{
  IO_init();
  LCD_Clear(BLACK);
  int ispress = 0;
  while (ispress == 0)
  {
    ispress = welcome();
    delay_1ms(100);
  }
  // welcome part

  LCD_Clear(BLACK);
  LCD_ShowString(10, 40, (u8 *)"Easy", WHITE);
  LCD_ShowString(10, 60, (u8 *)"Hard", GRAY);
  int isenter = 0;
  int difficulty = 0;
  int last_button = 7;
  int current_button = 7;
  uint32_t last_time = get_timer_value();
  uint32_t current_time = get_timer_value();
  while (isenter == 0)
  {
    int ischange = 0;
    if (Get_Button(JOY_RIGHT))
    {
      current_button = 2;
      current_time = get_timer_value();
      if (current_button == last_button && current_time - last_time < (SystemCoreClock / 4000.0 * 300))
      {
        continue;
      }
      else
      {
        ischange = 1;
        difficulty += 1;
        difficulty = difficulty % 2;
        last_button = current_button;
        last_time = current_time;
      }
    }
    else if (Get_Button(JOY_LEFT))
    {
      current_button = 0;
      current_time = get_timer_value();
      if (current_button == last_button && current_time - last_time < (SystemCoreClock / 4000.0 * 300))
      {
        continue;
      }
      else
      {
        ischange = 1;
        difficulty -= 1;
        if (difficulty < 0)
        {
          difficulty = 1;
        }
        last_button = current_button;
        last_time = current_time;
      }
    }
    else if (Get_Button(JOY_CTR))
    {
      isenter = 1;
    }
    if (difficulty == 0 && ischange == 1)
    {
      LCD_ShowString(10, 40, (u8 *)"Easy", WHITE);
      LCD_ShowString(10, 60, (u8 *)"Hard", GRAY);
    }
    else if (difficulty == 1 && ischange == 1)
    {
      LCD_ShowString(10, 40, (u8 *)"Easy", GRAY);
      LCD_ShowString(10, 60, (u8 *)"Hard", WHITE);
    }
  }
  // difficulty choice part

  LCD_Clear(BLACK);
  u16 lives = 999;
  u16 score = 0;
  u16 lives_len = 0;
  u16 score_len = 0;
  LCD_Fill(0, 0, 80, 40, BLACK);
  LCD_ShowString(0, 0, (u8 *)"Score:", WHITE);
  LCD_ShowString(0, 20, (u8 *)"Lives:", WHITE);
  struct Entity bird;
  struct Wall wall[2];
  struct Tail_dot tail[15];

  bird.x = 20;
  bird.y = 80;
  bird.v = 0;
  bird.color = GREEN;
  for (int i = 0; i <= 1; i++)
  {
    wall[i].x = LCD_W;
    if (difficulty == 0)
    {

      wall[i].y_gap = (rand() % (LCD_H - 40 - EASY_WALL_GAP)) + 40;
    }
    else
    {
      wall[i].y_gap = (rand() % (LCD_H - 40 - HARD_WALL_GAP)) + 40;
    }
  }
  for (int i = 0; i < 15; i++)
  {
    tail[i].color = BLACK;
    tail[i].x = bird.x - i - 1;
    tail[i].y = bird.y;
  }

  while (1)
  {
    uint64_t start_time = get_timer_value();
    int birdisup = 0;
    if (key_check())
    {
      if (difficulty == 0)
      {
        bird.v = EASY_upv;
      }
      else
      {
        bird.v = HARD_upv;
      }
      birdisup = 1;
    }
    UpdateEntity(&bird, birdisup, difficulty);
    UpdateWall(wall, difficulty);
    UpdateTail(tail, &bird);
    if (check_collision(&bird, wall, difficulty))
    {
      lives -= 1;
      for (int i = 14; i >= 0; i--)
      {
        DrawTail(&tail[i], &tail[i - 1], &bird, BLACK);
      }
      DrawEntity(&bird, BLACK);
      bird.x = 20;
      bird.y = 80;
      bird.v = 0;
      bird.color = GREEN;
      for (int i = 0; i < 15; i++)
      {
        tail[i].color = BLACK;
        tail[i].x = bird.x - i - 1;
        tail[i].y = bird.y;
      }
    }
    else
    {
      if (wall[0].x == bird.x)
      {
        score += 1;
      }
    }
    if (lives >= 100)
    {
      lives_len = 3;
    }
    else if (lives >= 10 && lives < 100)
    {
      lives_len = 2;
    }
    else if (lives > 0 && lives < 10)
    {
      lives_len = 1;
    }
    else if (lives_len == 0)
    {
      lives = 999;
      lives_len = 3;
    }

    if (score >= 100)
    {
      score_len = 3;
    }
    else if (score >= 10 && score < 100)
    {
      score_len = 2;
    }
    else if (score > 0 && score < 10)
    {
      score_len = 1;
    }
    LCD_ShowNum(45, 20, lives, lives_len, WHITE);
    LCD_ShowNum(45, 0, score, score_len, WHITE);
    uint64_t duration;
    do
    {
      duration = get_timer_value() - start_time;
    } while (duration < (SystemCoreClock / 4000.0 * 30));
  }
}
