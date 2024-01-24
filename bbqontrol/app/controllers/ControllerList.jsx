import React from "react";
import Link from "next/link";

async function getControllers() {
  const res = await fetch("http://localhost:4000/controllers");
  const json = await res.json();
  return json;
}

export default async function ControllerList() {
  const controllers = await getControllers();

  return (
    <>
      {controllers.map((controller) => (
        <div key={controller.id} className="controllerCard">
          <Link href={`/controllers/${controller.id}`}>
            <h3>{controller.name}</h3>
            <p>id: {controller.id}</p>
            <p>current temp: {controller.temp} °C</p>
            <p>fan speed: {controller.fanspeed} RPM oder %</p>
            <p>target temp: {controller.targettemp} °C</p>
          </Link>
        </div>
      ))}
    </>
  );
}
