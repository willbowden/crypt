#ifndef UI_H
#define UI_H

struct Game;
struct Sprite;

#define POPUP_HEIGHT 9
#define FONT_SIZE 16
#define MAX_MENU_ITEMS 8

typedef struct Popup {
  char *message;
  int visible;
} Popup;

typedef struct MenuItem {
  int (*select) (struct Game *g);
  char *text;
} MenuItem;

typedef struct Menu {
  MenuItem menu_items[MAX_MENU_ITEMS];
  struct Sprite *selector; 
  char *title;
  char *text;
  int num_items;
  int selected_item;
} Menu;

void show_popup(struct Game *game, char *message);
void dismiss_popup(struct Game *game);
void draw_dialog(struct Game *game);
void draw_hud(struct Game *game);
void draw_menu(struct Game *game);
void setup_game_over_menu(Menu *menu)
Menu *initialise_menu();
void cleanup_menu(Menu *menu);

#endif /* UI_H */
