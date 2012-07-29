DROP TABLE IF EXISTS `players`;
CREATE TABLE `players` 
(
	`guid` INT,
	`name` VARCHAR(20),
	`password` VARCHAR(30),
	`tileset` VARCHAR(20),
	`tx` SMALLINT UNSIGNED,
	`ty` SMALLINT UNSIGNED,
	`map` TINYINT UNSIGNED,
	`x` SMALLINT UNSIGNED,
	`y` SMALLINT UNSIGNED,
	PRIMARY KEY(guid)
);

INSERT INTO `players` VALUES (0, 'admin', 'password', 'dg_classm32.gif', 0, 0, 0, 5, 5);
INSERT INTO `players` VALUES (1, 'test', 'test', 'dg_classm32.gif', 1, 0, 0, 5, 6);

DROP TABLE IF EXISTS `creature_template`;
CREATE TABLE `creature_template`
(
	`entry` INT,
	`ScriptName` CHAR(192),
	PRIMARY KEY(entry)
)

INSERT INTO `creature_template` VALUES (0, 'npc_krofnica');

DROP TABLE IF EXISTS `creature`;
CREATE TABLE `creature`
(
	`guid` INT,
	`entry` INT,
	`map` TINYINT UNSIGNED,
	`x` TINYINT UNSIGNED,
	`y` TINYINT UNSIGNED,
	PRIMARY KEY(guid)
);

INSERT INTO `creature` VALUES (2, 0, 0, 10, 10);