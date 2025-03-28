struct Game;

typedef struct UI {
  char *message;
  int visible;
} UI;

void show_popup(struct Game *game, char *message);
void dismiss_popup(struct Game *game);
void draw_ui(struct Game *game);
UI *initialize_ui();
void cleanup_ui(UI *ui);