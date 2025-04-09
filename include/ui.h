struct Game;

#define POPUP_HEIGHT 9
#define FONT_SIZE 16

typedef struct UI {
  char *message;
  int visible;
} UI;

void show_popup(struct Game *game, char *message);
void dismiss_popup(struct Game *game);
void draw_dialog(struct Game *game);
void draw_hud(struct Game *game);
UI *initialize_ui();
void cleanup_ui(UI *ui);