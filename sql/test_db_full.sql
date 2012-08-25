DROP TABLE IF EXISTS `players`;
CREATE TABLE `players` 
(
	`guid` BIGINT UNSIGNED NOT NULL,
	`username` VARCHAR(20) NOT NULL,
	`password` VARCHAR(30) NOT NULL,
    `online` TINYINT UNSIGNED DEFAULT 0,
    `seclevel` TINYINT UNSIGNED DEFAULT 0,
	`tileset` VARCHAR(20) NOT NULL,
	`tx` SMALLINT UNSIGNED NOT NULL,
	`ty` SMALLINT UNSIGNED NOT NULL,
	`mapid` TINYINT UNSIGNED NOT NULL,
	`x` SMALLINT UNSIGNED NOT NULL,
	`y` SMALLINT UNSIGNED NOT NULL,
	PRIMARY KEY(guid)
);

DROP TABLE IF EXISTS `creature_template`;
CREATE TABLE `creature_template`
(
	`entry` INT UNSIGNED NOT NULL,
	`name` CHAR(192) NOT NULL,
	`tileset` VARCHAR(20) NOT NULL,
	`tx` SMALLINT UNSIGNED NOT NULL,
	`ty` SMALLINT UNSIGNED NOT NULL,
	`ScriptName` CHAR(192),
	PRIMARY KEY(entry)
);

INSERT INTO `creature_template` VALUES (0, 'krofnica', 'dg_classm32.gif', 7, 10, 'npc_krofnica');
INSERT INTO `creature_template` VALUES (1, 'The FullMetal Alchimist', 'dg_classm32.gif', 7, 10, 'npc_the_fullmetal_alchimist');

DROP TABLE IF EXISTS `creature`;
CREATE TABLE `creature`
(
	`guid` BIGINT UNSIGNED NOT NULL,
	`entry` INT UNSIGNED NOT NULL,
	`map` TINYINT UNSIGNED NOT NULL,
	`x` TINYINT UNSIGNED NOT NULL,
	`y` TINYINT UNSIGNED NOT NULL,
	PRIMARY KEY(guid)
);

INSERT INTO `creature` VALUES (UUID_SHORT(), 0, 0, 10, 10);
INSERT INTO `creature` VALUES (UUID_SHORT(), 1, 0, 11, 11);
