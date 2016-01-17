
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

import os

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    '''NOTE: This is something I added so that various emacs
             buffers would be removed with each build (a
	     build and clean, if you will)'''
    print 'Cleaning src directory...'
    
    filelist = [f for f in os.listdir("src/") if f.endswith(".c~")]
    
    for f in filelist:
        os.remove("src/" + f)

    filelist = [f for f in os.listdir("src/headers/") if f.endswith(".h~")]

    for f in filelist:
        os.remove("src/headers/" + f)
    

    ctx.load('pebble_sdk')

    build_worker = os.path.exists('worker_src')
    binaries = []

    for p in ctx.env.TARGET_PLATFORMS:
        ctx.set_env(ctx.all_envs[p])
        ctx.set_group(ctx.env.PLATFORM_NAME)
        app_elf='{}/pebble-app.elf'.format(ctx.env.BUILD_DIR)
        ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c'),
        target=app_elf)

        if build_worker:
            worker_elf='{}/pebble-worker.elf'.format(ctx.env.BUILD_DIR)
            binaries.append({'platform': p, 'app_elf': app_elf, 'worker_elf': worker_elf})
            ctx.pbl_worker(source=ctx.path.ant_glob('worker_src/**/*.c'),
            target=worker_elf)
        else:
            binaries.append({'platform': p, 'app_elf': app_elf})

    ctx.set_group('bundle')
    ctx.pbl_bundle(binaries=binaries, js=ctx.path.ant_glob('src/js/**/*.js'))
