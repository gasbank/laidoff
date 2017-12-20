print('post_init.lua visible')
local inspect = require('inspect')

-- Utility functions begin

function lo.new_vec3(x, y, z)
	local v = lo.new_float(3)
	lo.float_setitem(v, 0, x)
	lo.float_setitem(v, 1, y)
	lo.float_setitem(v, 2, z)
	return v
end

function lo.print_vec3(v)
	print('x', lo.float_getitem(v, 0), 'y', lo.float_getitem(v, 1), 'z', lo.float_getitem(v, 2))
end

function lo.get_vec3(v)
	return {
		['x'] = lo.float_getitem(v, 0),
		['y'] = lo.float_getitem(v, 1),
		['z'] = lo.float_getitem(v, 2)
	}
end

function lo.delete_vec3(v)
	lo.delete_float(v)
end

function reload_require(modname)
	package.loaded[modname] = nil
	return require(modname)
end

-- Utility functions end

local c = lo.script_context()

local Data = reload_require('data')
print('Data loaded!')
local data = Data:new()

local Field = reload_require('field')
print('Field loaded!')
local field = Field:new('test field')
field:test()
field:start_updating()
-- Lua handler for anim marker events emitted from C.
-- This function is called from C, thus should not have 'local' specifier.
-- Also note that this function implicitly captures 'field'.
function on_anim_marker(key, name)
	--print('on_anim_marker key:',key,', name:', name)
	field:on_anim_marker(key, name)
	return 0
end
-- Lua handler for collision events (near events) emitted from C.
function on_near(key1, key2)
	--print('on_near key1', key1, 'key2', key2)
	field.objs[key2].obj:collide_with(key1)
	return 0
end
-- Lua handler for logc frame finish events emitted from C
function on_logic_frame_finish(dt)
	field:update(dt)
	return 0
end

local last_construct_gtid = ''

function copy(obj, seen)
  if type(obj) ~= 'table' then return obj end
  if seen and seen[obj] then return seen[obj] end
  local s = seen or {}
  local res = setmetatable({}, getmetatable(obj))
  s[obj] = res
  for k, v in pairs(obj) do res[copy(k, s)] = copy(v, s) end
  return res
end

-- Lua handler for logc frame finish events emitted from C
function on_ui_event(id)
	print('ui event emitted from C:' .. id)
	local gtid = 'catapult'
	if id == 'seltower0' then gtid = 'catapult'
	elseif id == 'seltower1' then gtid = 'crossbow'
	elseif id == 'seltower2' then gtid = 'guntower'
	elseif id == 'seltower3' then gtid = 'pyro'
	elseif id == 'pull_button' then
		print('[script]button_pull')
		lo.puck_game_pull_puck_toggle(c, c.puck_game)
		return 0
	elseif id == 'dash_button' then
		print('[script]button_dash')
		lo.puck_game_dash_and_send(c, c.puck_game)
		return 0
	elseif id == 'jump_button' then
		print('[script]button_jump')
		lo.puck_game_jump(c, c.puck_game)
		-- temporarily for rematch
		lo.puck_game_rematch(c, c.puck_game)
		return 0
	elseif id == 'practice_button' then
		print('[script]practice_button')
		lo.puck_game_reset_battle_state(c.puck_game)
		lo.puck_game_clear_match_data(c, c.puck_game)
		lo.puck_game_reset_view_proj(c, c.puck_game)
		lo.puck_game_roll_to_practice(c.puck_game)
	elseif id == 'back_button' then
		print('[script]back_button')
		lo.puck_game_set_tower_invincible(c.puck_game, 0, 0)
		lo.puck_game_set_tower_invincible(c.puck_game, 1, 0)
		if c.puck_game.game_state == lo.LPGS_SEARCHING then
			lo.tcp_send_cancelqueue(c.tcp, c.tcp.user_id)
			c.puck_game.game_state = lo.LPGS_MAIN_MENU
		elseif c.game_scene == lo.LGS_LEADERBOARD then
			lo.change_to_physics(c)
		elseif c.puck_game.game_state == lo.LPGS_TUTORIAL then
			lo.script_cleanup_all_coros(c)
			--c.puck_game.game_state = lo.LPGS_MAIN_MENU
			lo.lw_go_back(c, c.android_native_activity)
		else
			lo.lw_go_back(c, c.android_native_activity)
		end
	elseif id == 'tutorial_button' then
		--lo.show_sys_msg(c.def_sys_msg, 'TUTORIAL: under construction')
		print('[script]tutorial_button')
		lo.puck_game_reset_tutorial_state(c.puck_game)
		lo.puck_game_clear_match_data(c, c.puck_game)
		lo.puck_game_reset_view_proj(c, c.puck_game)
		lo.puck_game_roll_to_tutorial(c.puck_game)
		start_coro(function()
			-- restore to full HP
			c.puck_game.target.current_hp = c.puck_game.hp;
			lo.puck_game_set_tower_invincible(c.puck_game, 1, 0)
			-- hide pull and dash buttons
			-- (left dir pad not already hidden since UI alpha is 0)
			c.puck_game.control_flags = c.puck_game.control_flags | lo.LPGCF_HIDE_PULL_BUTTON | lo.LPGCF_HIDE_DASH_BUTTON
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '환영합니다!')
			yield_wait_ms(1500)
			lo.puck_game_create_go(c.puck_game, lo.LPGO_PLAYER, 0, 0, 10);
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '왼쪽 <스틱>으로 플레이어를 움직여 보세요.')
			lo.puck_game_create_control_joint(c.puck_game, lo.LPGO_PLAYER)
			c.puck_game.battle_control_ui_alpha = 1
			yield_wait_ms(4000)
			lo.puck_game_create_go(c.puck_game, lo.LPGO_PUCK, 1.1, 1.1, 10);
			--------------------------------------
			local near_count = 2
			local near_msg = '흰색 공과 부딪쳐보세요. (%d/%d)'
			for i=1,near_count do
				lo.puck_game_set_tutorial_guide_str(c.puck_game, string.format(near_msg, i-1, near_count))
				if i ~= 1 then
					yield_wait_ms(500)
				end
				yield_wait_near_puck_player(1)
			end
			lo.puck_game_set_tutorial_guide_str(c.puck_game, string.format(near_msg, near_count, near_count))
			yield_wait_ms(1000)
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '잘했습니다!')
			yield_wait_ms(2000)
			--------------------------------------
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '오른쪽 <대시 버튼>으로 <대시>도 가능합니다.')
			yield_wait_ms(2000)
			c.puck_game.control_flags = c.puck_game.control_flags & ~lo.LPGCF_HIDE_DASH_BUTTON
			--------------------------------------
			local dash_near_count = 2
			local dash_near_msg = '<대시>를 사용해 공과 부딪쳐보세요. (%d/%d)'
			for i=1,dash_near_count do
				lo.puck_game_set_tutorial_guide_str(c.puck_game, string.format(dash_near_msg, i-1, dash_near_count))
				if i ~= 1 then
					yield_wait_ms(500)
				end
				yield_wait_near_puck_player_with_dash(1)
			end
			lo.puck_game_set_tutorial_guide_str(c.puck_game, string.format(dash_near_msg, dash_near_count, dash_near_count))
			yield_wait_ms(1000)
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '아주 잘했습니다!')
			yield_wait_ms(2000)
			--------------------------------------
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '<적군 타워>를 소환하겠습니다.')
			yield_wait_ms(2500)
			lo.puck_game_create_tower_geom(c.puck_game, 1);
			--------------------------------------
			local damage_count = 2
			local damage_msg = '공으로 <적군 타워>에 데미지를 입히세요. (%d/%d)'
			for i=1,damage_count do
				lo.puck_game_set_tutorial_guide_str(c.puck_game, string.format(damage_msg, i-1, damage_count))
				yield_wait_player_attack(1)
			end
			-- make enemy tower invincible for now
			lo.puck_game_set_tower_invincible(c.puck_game, 1, 1)
			lo.puck_game_set_tutorial_guide_str(c.puck_game, string.format(damage_msg, damage_count, damage_count))
			yield_wait_ms(1000)
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '대단히 잘했습니다!')
			yield_wait_ms(2000)
			--------------------------------------
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '마지막으로 <아군 타워>와 <적>을 소환하겠습니다.')
			yield_wait_ms(4000)
			lo.puck_game_create_tower_geom(c.puck_game, 0);
			lo.puck_game_create_go(c.puck_game, lo.LPGO_TARGET, 0, 1.1, 10);
			lo.puck_game_set_tutorial_guide_str(c.puck_game, '적의 방해를 피해 <적군 타워>를 파괴하세요!')
			-- remove guide text after some time passed
			start_coro(function()
				yield_wait_ms(2000)
				lo.puck_game_set_tutorial_guide_str(c.puck_game, '')
			end)
			-- make enemy tower can be damaged
			lo.puck_game_set_tower_invincible(c.puck_game, 1, 0)
			lo.puck_game_create_control_joint(c.puck_game, lo.LPGO_TARGET)
			yield_wait_target_attack(4)
			-- make player tower invincible at emergency
			lo.puck_game_set_tower_invincible(c.puck_game, 0, 1)
		end)
		
	elseif id == 'online_button' then
		c.puck_game.game_state = lo.LPGS_SEARCHING
		lo.puck_game_set_searching_str(c.puck_game, 'SEARCHING OPPONENT...')
		lo.puck_game_clear_match_data(c, c.puck_game)
		lo.puck_game_reset_battle_state(c.puck_game)
		lo.tcp_send_queue2(c.tcp, c.tcp.user_id)
	elseif id == 'leaderboard_button' then
		lo.show_leaderboard(c)
	else 
		lo.construct_set_preview_enable(c.construct, 0)
		return 0
	end
	local gt = data.guntower[gtid]
	if last_construct_gtid ~= gtid then
		lo.construct_set_preview_enable(c.construct, 1)
		lo.construct_set_preview(c.construct, gt.atlas, gt.skin_vbo, gt.armature, gt.anim_action_id)
	else
		local Faction1 = 1
		local gtinst = copy(gt)
		print(inspect(gtinst))
		local pt = lo.new_vec3(0, 0, 0)
		lo.field_get_player_position(c.field, pt)
		local ptvec = lo.get_vec3(pt)
		gtinst.x = ptvec.x
		gtinst.y = ptvec.y
		gtinst.z = ptvec.z
		field:spawn(gtinst, Faction1)
		gtinst:start_thinking()
	end
	last_construct_gtid = gtid
	return 0
end

local function split_path(p)
	return string.match(p, "(.-)([^\\/]-%.?([^%.\\/]*))$")
end
local field_filename = lo.field_filename(c.field)
if field_filename then
	print('Field filename:' .. field_filename)
	local field_filename_path, field_filename_name, field_filename_ext = split_path(field_filename)
	local field_module_name = string.sub(field_filename_name, 1, string.len(field_filename_name) - string.len(field_filename_ext) - 1)
	print('Field filename path:' .. field_filename_path)
	print('Field filename name:' .. field_filename_name)
	print('Field filename name only:' .. field_module_name)
	print('Field filename ext:' .. field_filename_ext)

	local FieldModule = reload_require(field_module_name)
	local field_module = FieldModule:new(field_filename_name, field)
	print('field_module:test()', field_module:test())
end
return 1
