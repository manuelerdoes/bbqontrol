import React from 'react'

async function getController(id) {
  const res = await fetch('http://localhost:4000/controllers/' + id, {
    next: {
      revalidate: 0
    }
  })
  const json = await res.json()
  return json
} 

export default async function ControllerDetail({ params }) {
  const controller = await getController(params.id);

  return (
    <main>
      <nav className='controllerDetailCard'>
        <h2>{controller.name}</h2>
      </nav>
      <div className='controllerDetailCard'>
        <p>id: {controller.id}</p>
        <p>current temp: {controller.temp} °C</p>
        <p>fan speed: {controller.fanspeed} RPM oder %</p>
        <p>target temp: {controller.targettemp} °C</p>
      </div>
    </main>
  )
}
