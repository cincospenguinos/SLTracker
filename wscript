#
# wscript
#
# Build script for sltracker. Uses waf. I tinkered around with this.

import os

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')

def configure(ctx):
    ctx.load('pebble_sdk')

def build(ctx):
    # I can't stand all the emacs *.*~ buffers
    print 'Cleaning src directory...'
    
    filelist = [f for f in os.listdir("src/rect") if f.endswith(".c~")]
    filelist.extend([f for f in os.listdir("src/chalk") if f.endswith(".c~")])
    filelist.extend([f for f in os.listdir("src/headers") if f.endswith(".h~")])
    
    for f in filelist:
        os.remove("src/" + f)
    
    print 'Building...'
    ctx.load('pebble_sdk')

    build_worker = os.path.exists('worker_src')
    binaries = []

    for p in ctx.env.TARGET_PLATFORMS:
        ctx.set_env(ctx.all_envs[p])
        ctx.set_group(ctx.env.PLATFORM_NAME)
        app_elf='{}/pebble-app.elf'.format(ctx.env.BUILD_DIR)

	# If we're doing chalk, then set the source to chalk. Otherwise, set the source to basalt
        if p == 'chalk':
            ctx.pbl_program(source=ctx.path.ant_glob(['src/chalk/**/*.c', 'src/common/**/*.c']),
            target=app_elf)
        else:
            ctx.pbl_program(source=ctx.path.ant_glob(['src/rect/**/*.c', 'src/common/**/*.c']),
            target=app_elf)

        if build_worker:
            worker_elf='{}/pebble-worker.elf'.format(ctx.env.BUILD_DIR)
            binaries.append({'platform': p, 'app_elf': app_elf, 'worker_elf': worker_elf})

	    if p == 'chalk':
	        ctx.pbl_worker(source=ctx.path.ant_glob(['worker_src/chalk/**/*.c', 'worker_src/common/**/*.c']), target=worker_elf)
            else:
                ctx.pbl_worker(source=ctx.path.ant_glob(['worker_src/rect/**/*.c', 'worker_src/common/**/*.c']), target=worker_elf)
        else:
            binaries.append({'platform': p, 'app_elf': app_elf})

    ctx.set_group('bundle')
    ctx.pbl_bundle(binaries=binaries, js=ctx.path.ant_glob('src/js/**/*.js'))
