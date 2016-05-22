<?php
// DIC configuration

$container = $app->getContainer();

// view renderer
$container['renderer'] = function ($c) {
    $settings = $c->get('settings')['renderer'];
    return new Slim\Views\PhpRenderer($settings['template_path']);
};

// monolog
$container['logger'] = function ($c) {
    $settings = $c->get('settings')['logger'];
    $logger = new Monolog\Logger($settings['name']);
    $logger->pushProcessor(new Monolog\Processor\UidProcessor());
    $logger->pushHandler(new Monolog\Handler\StreamHandler($settings['path'], Monolog\Logger::DEBUG));
    return $logger;
};
$container['db'] = function($c){

	$conn = null;

    $db_host = $c['settings']['db']['host'];  //Database Host
    $db_name = $c['settings']['db']['database']; //Database Name
    $db_user = $c['settings']['db']['user'];  //Database User
    $user_pw = $c['settings']['db']['password'];  //Database Password

    $con = new PDO('mysql:host='.$db_host.'; dbname='.$db_name, $db_user, $user_pw);
    $con->setAttribute( PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION );
    $con->exec("SET CHARACTER SET utf8");  //  return all sql requests as UTF-8

	return $conn;

};
