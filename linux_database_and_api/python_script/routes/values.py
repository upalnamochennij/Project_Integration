from fastapi import APIRouter
from database import database
from datetime import date
from datetime import datetime
from pydantic import BaseModel

router = APIRouter()
#basemodels
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
    query = """
    INSERT INTO steps (timestamp, device_id, steps)
    SELECT :timestamp, :device_id, :steps
    WHERE EXISTS (
        SELECT 1 FROM Devices WHERE device_id = :device_id
    )
    AND NOT EXISTS (
        SELECT 1 FROM steps WHERE timestamp = :timestamp AND device_id = :device_id
    );
    """
    values = {
        "timestamp": timestamp,
        "device_id": device_id,
        "steps": steps,
    }
    try:
        result = await database.execute(query=query, values=values)
        if result is None:
            return {"message": "Device does not exist or record already exists (is not possible or your adding it twice somehow)"}
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
    query = """
    INSERT INTO MPU_information (
        timestamp, device_id, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z
    )
    SELECT
        :timestamp, :device_id, :gyro_x, :gyro_y, :gyro_z, :accel_x, :accel_y, :accel_z
    WHERE EXISTS (
        SELECT 1 FROM Devices WHERE device_id = :device_id
    )
    AND NOT EXISTS (
        SELECT 1 FROM MPU_information WHERE timestamp = :timestamp AND device_id = :device_id
    );
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
        result = await database.execute(query=query, values=values)
        if result is None:
            return {"message": "Device does not exist or record already exists"}
        return {"message": "MPU data added successfully"}
    except Exception as e:
        return {"message": f"Failed to add MPU data: {str(e)}"}
    
@router.get("/add_temperature")
async def add_temperature(
    timestamp: datetime,
    device_id: int,
    temperature: int,
):
    query = """
    INSERT INTO temperature (timestamp, device_id, temperature)
    SELECT :timestamp, :device_id, :temperature
    WHERE EXISTS (
        SELECT 1 FROM Devices WHERE device_id = :device_id
    )
    AND NOT EXISTS (
        SELECT 1 FROM temperature WHERE timestamp = :timestamp AND device_id = :device_id
    );
    """
    values = {
        "timestamp": timestamp,
        "device_id": device_id,
        "temperature": temperature,
    }
    try:
        result = await database.execute(query=query, values=values)
        if result is None:
            return {"message": "Device does not exist or record already exists"}
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
    query = """
    INSERT INTO pulse_oxygen (timestamp, device_id, SPO2, heartrate)
    SELECT :timestamp, :device_id, :SPO2, :heartrate
    WHERE EXISTS (
        SELECT 1 FROM Devices WHERE device_id = :device_id
    )
    AND NOT EXISTS (
        SELECT 1 FROM pulse_oxygen WHERE timestamp = :timestamp AND device_id = :device_id
    );
    """
    values = {
        "timestamp": timestamp,
        "device_id": device_id,
        "SPO2": SPO2,
        "heartrate": heartrate,
    }
    try:
        result = await database.execute(query=query, values=values)
        if result is None:
            return {"message": "Device does not exist or record already exists"}
        return {"message": "PulseOxygen data added successfully"}
    except Exception as e:
        return {"message": f"Failed to add PulseOxygen data: {str(e)}"}

#retrieving data if device exists
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
