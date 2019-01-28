<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInitComposerManager
{
    public static $files = array (
        '0e6d7bf4a5811bfa5cf40c5ccd6fae6a' => __DIR__ . '/..' . '/symfony/polyfill-mbstring/bootstrap.php',
    );

    public static $prefixLengthsPsr4 = array (
        'S' => 
        array (
            'Symfony\\Polyfill\\Mbstring\\' => 26,
            'Symfony\\Component\\Debug\\' => 24,
            'Symfony\\Component\\Console\\' => 26,
        ),
        'P' => 
        array (
            'Psr\\Log\\' => 8,
        ),
        'D' => 
        array (
            'Drupal\\amazons3\\' => 16,
            'Drupal\\amazons3Test\\' => 20,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'Symfony\\Polyfill\\Mbstring\\' => 
        array (
            0 => __DIR__ . '/..' . '/symfony/polyfill-mbstring',
        ),
        'Symfony\\Component\\Debug\\' => 
        array (
            0 => __DIR__ . '/..' . '/symfony/debug',
        ),
        'Symfony\\Component\\Console\\' => 
        array (
            0 => __DIR__ . '/..' . '/symfony/console',
        ),
        'Psr\\Log\\' => 
        array (
            0 => __DIR__ . '/..' . '/psr/log/Psr/Log',
        ),
        'Drupal\\amazons3\\' => 
        array (
            0 => __DIR__ . '/../../..'.'/default/files/composer' . '/../../../all/modules/contrib/amazons3/src',
        ),
        'Drupal\\amazons3Test\\' => 
        array (
            0 => __DIR__ . '/../../..'.'/default/files/composer' . '/../../../all/modules/contrib/amazons3/tests',
        ),
    );

    public static $prefixesPsr0 = array (
        'S' => 
        array (
            'SensioLabs\\Security' => 
            array (
                0 => __DIR__ . '/..' . '/sensiolabs/security-checker',
            ),
        ),
    );

    public static $classMap = array (
        'DrupalStreamWrapperInterface' => __DIR__ . '/../../..'.'/default/files/composer' . '/../../../all/modules/contrib/amazons3/tests/include/DrupalStreamWrapperInterface.inc',
        'StreamWrapperInterface' => __DIR__ . '/../../..'.'/default/files/composer' . '/../../../all/modules/contrib/amazons3/tests/include/DrupalStreamWrapperInterface.inc',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInitComposerManager::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInitComposerManager::$prefixDirsPsr4;
            $loader->prefixesPsr0 = ComposerStaticInitComposerManager::$prefixesPsr0;
            $loader->classMap = ComposerStaticInitComposerManager::$classMap;

        }, null, ClassLoader::class);
    }
}
