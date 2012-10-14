-- Placeholder database used for testing various game aspects

-- Having test account saves alot of trouble
INSERT INTO `players` VALUES (UUID_SHORT(), 'test', 'test', 'none', 0, 0, 'dg_classm32.gif', 0, 0, 0, 0, 0);
INSERT INTO `map` VALUES(0, 1);
INSERT INTO `spell_template` VALUES (DEFAULT, 1, 0, 10, 10, 'Bolt');
INSERT INTO `spell_template` VALUES (DEFAULT, 1, 1, 0, 0, 'Quadruple Accel No Interval');
INSERT INTO `gameobject_template` VALUES (DEFAULT, 'test_go', 'dg_edging132.gif', 7, 15, 0, 'go_test_go');
INSERT INTO `gameobject` VALUES (UUID_SHORT(), 1, 0, 9, 9);
INSERT INTO `map_template` VALUES (DEFAULT, 30, 20, 0, 'map_test_map');
INSERT INTO `item_template` VALUES (DEFAULT, 1, 1, "Krofna's bag", 1, 1, 36, "The bottom of the crate is leaking. Leaking tears...");
INSERT INTO `creature_template` VALUES (DEFAULT, 'krofnica', 'dg_classm32.gif', 7, 10, 8, 100, 100, 'npc_krofnica');
INSERT INTO `creature_template` VALUES (DEFAULT, 'The FullMetal Alchimist', 'dg_classm32.gif', 7, 10, 0, 100, 100, 'npc_the_fullmetal_alchimist');
INSERT INTO `creature` VALUES (UUID_SHORT(), 1, 0, 10, 10);
INSERT INTO `creature` VALUES (UUID_SHORT(), 2, 0, 11, 11);
INSERT INTO `quest_template` VALUES (DEFAULT, "Help!", "One of our developers, has gone rogue! Find and eliminate him!", "Kill FullMetal-Alchimist");
INSERT INTO `quest_template` VALUES (DEFAULT, "Help!", "One of our developers, don't know how to make a performance analysis! Please find and teach him !", "Teach how to make a performance analysis to Krofna");
INSERT INTO `creature_quest` VALUES (1, 1);
