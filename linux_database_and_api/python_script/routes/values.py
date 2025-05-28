from fastapi import APIRouter
from database import database
from datetime import date
from datetime import datetime
from pydantic import BaseModel

router = APIRouter()
#basemodels for sending back
class MPUInformation(BaseModel):
    timestamp: datetime
    device_id: int
    gyro_x: int
    gyro_y: int
    gyro_z: int
    accel_x: int
    accel_y: int
    accel_z: int

class Temperature(BaseModel):
    timestamp: datetime
    device_id: int
    temperature: int

class PulseOxygen(BaseModel):
    timestamp: datetime
    device_id: int
    SPO2: int
    heartrate: int

class Steps(BaseModel):
    timestamp: datetime
    device_id: int
    steps: int

#insertion if device exists
@router.get("/add_steps")
async def add_steps(
    timestamp: datetime,
    device_id: int,
    steps: int
):
    # does device excist?
    device_check_query = "SELECT 1 FROM Devices WHERE device_id = :device_id"
    device_exists = await database.fetch_one(query=device_check_query, values={"device_id": device_id})
    
    if not device_exists:
        return {"message": "Device does not exist"}

    # are you trying to add a record multiple times?
    record_check_query = """
    SELECT 1 FROM steps WHERE timestamp = :timestamp AND device_id = :device_id
    """
    record_exists = await database.fetch_one(query=record_check_query, values={
        "timestamp": timestamp,
        "device_id": device_id
    })

    if record_exists:
        return {"message": "Steps record already exists for this timestamp and device"}

    # now you can insert
    insert_query = """
    INSERT INTO steps (timestamp, device_id, steps)
    VALUES (:timestamp, :device_id, :steps)
    """
    try:
        await database.execute(query=insert_query, values={
            "timestamp": timestamp,
            "device_id": device_id,
            "steps": steps
        })
        return {"message": "Steps data added successfully"}
    except Exception as e:
        return {"message": f"Failed to add steps data: {str(e)}"}

    
@router.get("/add_MPU")
async def add_mpu(
    timestamp: datetime,
    device_id: int,
    gyro_x: int,
    gyro_y: int,
    gyro_z: int,
    accel_x: int,
    accel_y: int,
    accel_z: int,
):
    # does device excist?
    device_check_query = "SELECT 1 FROM Devices WHERE device_id = :device_id"
    device_exists = await database.fetch_one(query=device_check_query, values={"device_id": device_id})
    
    if not device_exists:
        return {"message": "Device does not exist"}

    # are you trying to add one record multiple times?
    record_check_query = """
    SELECT 1 FROM MPU_information WHERE timestamp = :timestamp AND device_id = :device_id
    """
    record_exists = await database.fetch_one(query=record_check_query, values={
        "timestamp": timestamp,
        "device_id": device_id
    })

    if record_exists:
        return {"message": "MPU record already exists for this timestamp and device"}

    # now you can insert
    insert_query = """
    INSERT INTO MPU_information (
        timestamp, device_id, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z
    )
    VALUES (
        :timestamp, :device_id, :gyro_x, :gyro_y, :gyro_z, :accel_x, :accel_y, :accel_z
    )
    """
    values = {
        "timestamp": timestamp,
        "device_id": device_id,
        "gyro_x": gyro_x,
        "gyro_y": gyro_y,
        "gyro_z": gyro_z,
        "accel_x": accel_x,
        "accel_y": accel_y,
        "accel_z": accel_z,
    }

    try:
        await database.execute(query=insert_query, values=values)
        return {"message": "MPU data added successfully"}
    except Exception as e:
        return {"message": f"Failed to add MPU data: {str(e)}"}
    
@router.get("/add_temperature")
async def add_temperature(
    timestamp: datetime,
    device_id: int,
    temperature: int,
):
    # does the device exist?
    device_check_query = "SELECT 1 FROM Devices WHERE device_id = :device_id"
    device_exists = await database.fetch_one(
        query=device_check_query,
        values={"device_id": device_id}
    )
    
    if not device_exists:
        return {"message": "Device does not exist"}

    # are you trying to insert one value twice?
    record_check_query = """
    SELECT 1 FROM temperature WHERE timestamp = :timestamp AND device_id = :device_id
    """
    record_exists = await database.fetch_one(
        query=record_check_query,
        values={"timestamp": timestamp, "device_id": device_id}
    )

    if record_exists:
        return {"message": "Temperature record already exists for this timestamp and device"}

    # now you can insert
    insert_query = """
    INSERT INTO temperature (timestamp, device_id, temperature)
    VALUES (:timestamp, :device_id, :temperature)
    """
    try:
        await database.execute(
            query=insert_query,
            values={
                "timestamp": timestamp,
                "device_id": device_id,
                "temperature": temperature
            }
        )
        return {"message": "Temperature data added successfully"}
    except Exception as e:
        return {"message": f"Failed to add temperature data: {str(e)}"}

    
@router.get("/add_PulseOxygen")
async def add_PulseOxygen(
    timestamp: datetime,
    device_id: int,
    SPO2: int,
    heartrate: int
):
    # does the device exist?
    device_check_query = "SELECT 1 FROM Devices WHERE device_id = :device_id"
    device_exists = await database.fetch_one(
        query=device_check_query,
        values={"device_id": device_id}
    )
    
    if not device_exists:
        return {"message": "Device does not exist"}

    # are you trying to insert one value twice?
    record_check_query = """
    SELECT 1 FROM pulse_oxygen WHERE timestamp = :timestamp AND device_id = :device_id
    """
    record_exists = await database.fetch_one(
        query=record_check_query,
        values={"timestamp": timestamp, "device_id": device_id}
    )

    if record_exists:
        return {"message": "PulseOxygen record already exists for this timestamp and device"}

    # now you can insert
    insert_query = """
    INSERT INTO pulse_oxygen (timestamp, device_id, SPO2, heartrate)
    VALUES (:timestamp, :device_id, :SPO2, :heartrate)
    """
    values = {
        "timestamp": timestamp,
        "device_id": device_id,
        "SPO2": SPO2,
        "heartrate": heartrate,
    }

    try:
        await database.execute(query=insert_query, values=values)
        return {"message": "PulseOxygen data added successfully"}
    except Exception as e:
        return {"message": f"Failed to add PulseOxygen data: {str(e)}"}


#retrieving data if device exists

#where clause for filtering each value
where_clause = " WHERE timestamp >= :start_time AND timestamp <= :end_time AND device_id == :device_id"

@router.get("/get_values")
async def get_values():
    try:
        query_mpu = "SELECT * FROM MPU_information"
        query_temp = "SELECT * FROM temperature"
        query_pulse = "SELECT * FROM pulse_oxygen"
        query_steps = "SELECT * FROM steps"

        mpu_data = await database.fetch_all(query=query_mpu)
        temp_data = await database.fetch_all(query=query_temp)
        pulse_data = await database.fetch_all(query=query_pulse)
        steps_data = await database.fetch_all(query=query_steps)

        return {
            "mpu_information": [dict(record) for record in mpu_data],
            "temperature": [dict(record) for record in temp_data],
            "pulse_oxygen": [dict(record) for record in pulse_data],
            "steps": [dict(record) for record in steps_data]
        }
    except Exception as e:
        return {"message": f"Failed to fetch sensor data: {str(e)}"}
    
@router.get("/get_MPU_information")
async def get_MPU_information(
    device_id: int,
    start_time: datetime,
    end_time: datetime
):
    try:
        values = {
            "device_id": device_id,
            "start_time": start_time,
            "end_time": end_time
        }

        query_mpu = f"SELECT * FROM MPU_information{where_clause}"

        mpu_data = await database.fetch_all(query=query_mpu, values=values)

        return {
            "mpu_information": [dict(record) for record in mpu_data],
        }
    except Exception as e:
        return {"message": f"Failed to fetch sensor data: {str(e)}"}
    
@router.get("/get_temperature")
async def get_temperature(
    device_id: int,
    start_time: datetime,
    end_time: datetime
):
    try:
        values = {
            "device_id": device_id,
            "start_time": start_time,
            "end_time": end_time
        }

        query_temp = f"SELECT * FROM temperature{where_clause}"

        temp_data = await database.fetch_all(query=query_temp, values=values)

        return {
           "temperature": [dict(record) for record in temp_data],
        }
    except Exception as e:
        return {"message": f"Failed to fetch sensor data: {str(e)}"}
    
@router.get("/get_pulse_oxygen")
async def get_pulse_oxygen(
    device_id: int,
    start_time: datetime,
    end_time: datetime
):
    try:
        values = {
            "device_id": device_id,
            "start_time": start_time,
            "end_time": end_time
        }

        query_pulse = f"SELECT * FROM pulse_oxygen{where_clause}"

        pulse_data = await database.fetch_all(query=query_pulse, values=values)

        return {
            "pulse_oxygen": [dict(record) for record in pulse_data],
        }
    except Exception as e:
        return {"message": f"Failed to fetch sensor data: {str(e)}"}
    
@router.get("/get_steps")
async def get_steps(
    device_id: int,
    start_time: datetime,
    end_time: datetime
):
    try:
        values = {
            "device_id": device_id,
            "start_time": start_time,
            "end_time": end_time
        }

        query_steps = f"SELECT * FROM steps{where_clause}"

        steps_data = await database.fetch_all(query=query_steps, values=values)

        return {
            "steps": [dict(record) for record in steps_data]
        }
    except Exception as e:
        return {"message": f"Failed to fetch sensor data: {str(e)}"}
