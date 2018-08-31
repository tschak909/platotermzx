
#ifndef HELP_H
#define HELP_H

/** 
 * Clear help line
 */
void help_clear(void);

/**
 * Show the input prompt for help 
 */
void help_prompt_input(const char* text);

/**
 * Show the resulting key gained from prompt input
 */
void help_prompt_key_display(const char* text, char extend);

/**
 * Show available PLATO keys to get help.
 */
void help_keys(void);

/**
 * Return if entered string matches help entry
 */
void help_matches(char* entry, const char* match);

/**
 * Help entrypoint 
 */
void help_run(void);

#endif /* HELP_H */
