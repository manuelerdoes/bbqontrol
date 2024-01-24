import React from 'react'
import ControllerList from './ControllerList'

export default function controllers() {
  return (
    <main className='controllerOverview'>
      <nav>
        <div>
          <h2>Controllers</h2>
        </div>
      </nav>

    <ControllerList />

    </main>
  )
}
