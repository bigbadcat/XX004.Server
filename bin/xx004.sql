/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50556
Source Host           : localhost:3306
Source Database       : xx004

Target Server Type    : MYSQL
Target Server Version : 50556
File Encoding         : 65001

Date: 2019-01-30 17:43:43
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `tb_role`
-- ----------------------------
DROP TABLE IF EXISTS `tb_role`;
CREATE TABLE `tb_role` (
  `id` bigint(20) NOT NULL DEFAULT '0',
  `user` varchar(64) DEFAULT NULL,
  `server` int(11) DEFAULT NULL,
  `prof` int(11) DEFAULT NULL,
  `name` varchar(64) DEFAULT '',
  `level` int(11) DEFAULT '0',
  `exp` int(11) DEFAULT '0',
  `money` int(11) DEFAULT '0',
  `create_time` bigint(20) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_role
-- ----------------------------
INSERT INTO `tb_role` VALUES ('1', 'abc', '1', '1', 'hhh', '1', '0', '0', null);
INSERT INTO `tb_role` VALUES ('2', 'abc', '2', '1', 'kkk', '10', '300', '4050', null);
INSERT INTO `tb_role` VALUES ('3', 'abc', '1', '1', 'ddd', '1', '0', '0', '0');
INSERT INTO `tb_role` VALUES ('214748364734', 'dgc', '1', '0', 'kkk', '10', '300', '1000', '14214748364734');

-- ----------------------------
-- Table structure for `tb_skill`
-- ----------------------------
DROP TABLE IF EXISTS `tb_skill`;
CREATE TABLE `tb_skill` (
  `role_id` bigint(20) NOT NULL,
  `skill_id` int(11) NOT NULL,
  `level` int(11) NOT NULL,
  PRIMARY KEY (`role_id`,`skill_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_skill
-- ----------------------------
INSERT INTO `tb_skill` VALUES ('1', '1', '2');
INSERT INTO `tb_skill` VALUES ('214748364734', '2001', '15');

-- ----------------------------
-- Table structure for `tb_user`
-- ----------------------------
DROP TABLE IF EXISTS `tb_user`;
CREATE TABLE `tb_user` (
  `name` varchar(64) NOT NULL,
  `create_time` bigint(20) DEFAULT '0',
  `free_time` bigint(20) DEFAULT '0',
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tb_user
-- ----------------------------
INSERT INTO `tb_user` VALUES ('abc', '142522', '0');
INSERT INTO `tb_user` VALUES ('abcd', '12', '0');
INSERT INTO `tb_user` VALUES ('abcdef', '14252', '0');
INSERT INTO `tb_user` VALUES ('abcsadfs', '1548838875', '0');
INSERT INTO `tb_user` VALUES ('dgc', '1548838875', '0');
INSERT INTO `tb_user` VALUES ('sdgh', '3543', '678');
INSERT INTO `tb_user` VALUES ('xx123', '1548838890', '0');
INSERT INTO `tb_user` VALUES ('xx124', '1548841170', '0');

-- ----------------------------
-- Procedure structure for `sp_insert_update_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_insert_update_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insert_update_role`(IN `in_id` bigint,IN `in_name` varchar(64),IN `in_level` int,IN `in_exp` int,IN `in_money` int,IN `in_create_time` bigint)
BEGIN 
	INSERT INTO tb_role(id, name, level, exp, money, create_time)
	VALUES(in_id, in_name, in_level, in_exp, in_money, in_create_time)
	ON DUPLICATE KEY UPDATE id=in_id, name=in_name, level=in_level, exp=in_exp, money=in_money, create_time=in_create_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_insert_update_skill`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_insert_update_skill`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insert_update_skill`(IN `in_role_id` bigint,IN `in_skill_id` int,IN `in_level` int)
BEGIN 
	INSERT INTO tb_skill(role_id, skill_id, level)
	VALUES(in_role_id, in_skill_id, in_level)
	ON DUPLICATE KEY UPDATE role_id=in_role_id, skill_id=in_skill_id, level=in_level;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_insert_update_user`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_insert_update_user`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_insert_update_user`(IN `in_name` varchar(64),IN `in_create_time` bigint,IN `in_free_time` bigint)
BEGIN
	INSERT INTO tb_user(name, create_time, free_time)
	VALUES(in_name, in_create_time, in_free_time)
	ON DUPLICATE KEY UPDATE name=in_name, create_time=in_create_time, free_time=in_free_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_select_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_role`(IN `in_id` bigint)
BEGIN
	SELECT * FROM tb_role WHERE id = in_id;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_select_role_skill`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_role_skill`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_role_skill`(IN `in_rid` bigint)
BEGIN
	SELECT * FROM tb_skill WHERE role_id = in_rid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_select_user`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_user`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_user`(IN `in_name` varchar(64))
BEGIN
	SELECT * FROM tb_user WHERE name = in_name;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_select_user_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_select_user_role`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_select_user_role`(IN `in_name` varchar(64),IN `in_server` int)
BEGIN
	SELECT * FROM tb_role WHERE user = in_name AND server=in_server;
END
;;
DELIMITER ;
