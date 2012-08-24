DROP TABLE IF EXISTS `players`;
CREATE TABLE `players` 
(
	`guid` INT UNSIGNED NOT NULL AUTO_INCREMENT,
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

INSERT INTO `players` VALUES (DEFAULT, 'admin', 'password', 0, 4, 'dg_classm32.gif', 0, 0, 0, 5, 5);
INSERT INTO `players` VALUES (DEFAULT, 'test', 'test', 0, 0, 'dg_classm32.gif', 1, 0, 0, 5, 6);

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

DROP TABLE IF EXISTS `creature`;
CREATE TABLE `creature`
(
	`guid` INT UNSIGNED NOT NULL AUTO_INCREMENT,
	`entry` INT UNSIGNED NOT NULL,
	`map` TINYINT UNSIGNED NOT NULL,
	`x` TINYINT UNSIGNED NOT NULL,
	`y` TINYINT UNSIGNED NOT NULL,
	PRIMARY KEY(guid)
);

INSERT INTO `creature` VALUES (DEFAULT, 0, 0, 10, 10);
