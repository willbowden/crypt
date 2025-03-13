/* Based on spreadsheet tile number */
typedef enum {
    MAGE=74,
    GOBLIN=128,
    SPIDER=275,
    GHOST=321,
    GOON=124
} EnemyType;

struct Enemy;
struct Game;

typedef enum {
    ABILITY_NONE,
    ABILITY_TANK, /* Extra health/shield */
    ABILITY_INVISIBILITY,
    ABILITY_TELEPORTATION,
    ABILITY_SHARPNESS /* Extra damage */,
    ABILITY_POISON /* Long range attack */,
    ABILITY_FIREBALL /* Long range attack */
} AbilityType;

/* Function pointer for executing the special ability */
typedef void (*AbilityFunction) (struct Enemy *enemy, struct Game *game);

typedef struct {
    AbilityType type;
    AbilityFunction execute;
    int cooldown; /* Number of turns after which the enemy can use its ability. -1 implies no cooldown */
} SpecialAbility;

struct Enemy *add_enemy(struct Game *game, int tileNo);
void template_execution(struct Enemy *enemy, struct Game *game);