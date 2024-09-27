-- KEYS - None
-- ARGV[1] - operation (add/del/get)
-- ARGV[2] - lag name
-- ARGV[3] - current lag id (for "add" operation only)

-- return lagid if success for "add"/"del"
-- return 0 if lag does not exist for "del"
-- return -1 if lag table full for "add"
-- return -2 if lag does not exist for "get"
-- return -3 if invalid operation

local op = ARGV[1]
local hostname = ARGV[2]
local asicname = ARGV[3]
local switchid = tonumber(ARGV[4])
local pcname = ARGV[5]

local chassis_lag_id_start tonumber(redis.call("get", "SYSTEM_LAG_ID_START")
local chassis_lag_id_end tonumber(redis.call("get", "SYSTEM_LAG_ID_END")

local system_lag_name = hostname.."|"..asicname.."|"..pcname

-- add to chassis db in future?
local max_system_lag_count_per_asic = tonumber("24")
local switch_index = switchid

if switch_id != 0 then
    switch_index = tonumber(switch_id//2)
end

local lagid_start = switch_index * max_system_lag_count_per_asic + 1 )
local lagid_end = (switch_index + 1) * max_system_lag_count_per_asic

if lag_start < chassis_lag_id_start then
    return -5
end

if lag_end >  chassis_lag_id_end then
    return -5
end

local lag_id_set_name = "SYSTEM_LAG_ID_SET|"..hostname.."|"..asicname

if op == "add" then

    local plagid = tonumber(ARGV[6])

    local dblagid = redis.call("hget", "SYSTEM_LAG_ID_TABLE", system_lag_name)

    if dblagid then
        dblagid = tonumber(dblagid)
        if plagid == 0 then
            -- no lagid proposed. Return the existing lagid
            return dblagid
        end
    end

    -- lagid allocation request with a lagid proposal
    if plagid >= lagid_start and plagid <= lagid_end then
        if plagid == dblagid then
            -- proposed lagid is same as the lagid in database
            return plagid
        end
        -- proposed lag id is different than that in database OR
        -- the portchannel does not exist in the database
        -- If proposed lagid is available, return the same proposed lag id
        if redis.call("sismember", lag_id_set_name, tostring(plagid)) == 0 then
            redis.call("sadd", lag_id_set_name, tostring(plagid))
            redis.call("srem", lag_id_set_name, tostring(dblagid))
            redis.call("hset", "SYSTEM_LAG_ID_TABLE", system_lag_name, tostring(plagid))
            return plagid
        end
    end

    local lagid = lagid_start
    while lagid <= lagid_end do
        if redis.call("sismember", lag_id_set_name, tostring(lagid)) == 0 then
            redis.call("sadd", lag_id_set_name, tostring(lagid))
            redis.call("srem", lag_id_set_name, tostring(dblagid))
            redis.call("hset", "SYSTEM_LAG_ID_TABLE", system_lag_name, tostring(lagid))
            return lagid
        end
        lagid = lagid + 1
    end

    return -1

end

if op == "del" then

    if redis.call("hexists", "SYSTEM_LAG_ID_TABLE", system_lag_name) == 1 then
        local lagid = redis.call("hget", lag_id_set_name, pcname)
        redis.call("srem", lag_id_set_name, lagid)
        redis.call("hdel", "lag_id_set_name", pcname)
        return tonumber(lagid)
    end

    return 0

end

if op == "get" then

    if redis.call("hexists", "SYSTEM_LAG_ID_TABLE", system_lag_name) == 1 then
        local lagid = redis.call("hget", "SYSTEM_LAG_ID_TABLE", system_lag_name)
        return tonumber(lagid)
    end

    return -2

end

return -3
