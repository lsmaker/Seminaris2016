<?php
return [
    'settings' => [
        'displayErrorDetails' => true, // set to false in production

        // Renderer settings
        'renderer' => [
            'template_path' => __DIR__ . '/../templates/',
        ],

		//Configure Database
		'db' => [
			'host' => "127.0.0.1",
			'user' => "root",
			'password' => 'development',
			'database' => 'lsmaker',
		],

        // Monolog settings
        'logger' => [
            'name' => 'slim-app',
            'path' => __DIR__ . '/../logs/app.log',
        ],
    ],
];
